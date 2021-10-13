// vim: set ts=4:
// Data from Android-App: "Windsport Tracker"
//      <trkpt lat="53.151177128" lon="8.696178791">
//        <ele>38</ele>
//        <time>2021-06-20T15:33:07+02:00</time>
//        <course>165</course>
//        <speed>3.0915164</speed>
//        <geoidheight>null</geoidheight>
//        <src>gps</src>
//        <sat>18</sat>
//        <hdop>0.8</hdop>
//        <vdop>1.1</vdop>
//        <pdop>1.4</pdop>
//        <trkptwst accuracy="6" elapsedrealtimenanos="0" />
//      </trkpt>


#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib> // EXIT_SUCCESS,...
#include <cstring> // strstr
#include <cstdio>
#include <getopt.h>
#include <vector>

#include "file.hpp"
#include "parse.hpp"
#include "image.hpp"
#include "option.hpp"

#define BUF 256


enum GPX_TOKEN { GPX_TRKPT, GPX_TIME, GPX_COURSE, GPX_SPEED, GPX_END, GPX_IGNORE };


std::vector<char*>* splitArg( const char* arg );
void pUsage( void );
void pError( const char* msg );


char buffer[BUF];
File Gpx;
Option ops;
GpxImage Gi;
char defaultLayer[] = "b0g0a1";




int main( int argc, char* argv[] )
{
	if( !ops.load( argc, argv ) )
		pError( "not properly configured" );
	
	if( optind+1 != argc )
		pUsage();
	//std::cout << "inputfile: " << argv[optind] << std::endl;
	
	if( !Gpx.open( argv[optind] ) )
		pError( "Can not open GPX file" );
	
	// prepare options
	std::vector<char*>* speedOps;
	char speedLayer[7];
	float speedMax;
	float speedScale;
	std::vector<char*>* speedTextOps;
	char speedText[128] = {0};
	int speedTextSize = 0;
	int speedTextX = 0;
	int speedTextY = 0;
	if( ops.isSet( OPT_SPEED ) )
	{
		// split option argument into vector ( delimeter=,)
		speedOps = splitArg( ops.getArg( OPT_SPEED ) );
		// check argument count / vector.size()
		if( 3!=speedOps->size() )
			pError( "wrong argument count for option: --speed" );
		// copy leyerDef or defaultLayer into variable
		if( 0 == strcmp( "d", speedOps->at(0) ) )
			strcpy( speedLayer, defaultLayer );
		else
			strcpy( speedLayer, speedOps->at(0) );
		// convert string into float
		sscanf( speedOps->at(1), "%f", &speedMax );
		sscanf( speedOps->at(2), "%f", &speedScale );
		
		// additional options for speed
		if( ops.isSet( OPT_SPEEDTEXT ) )
		{
			speedTextOps = splitArg( ops.getArg( OPT_SPEEDTEXT ) );
			if( 4!=speedTextOps->size() )
				pError( "wrong argument count for option: --speed_text" );
			strcpy( speedText, speedTextOps->at(0) );
			sscanf( speedTextOps->at(1), "%i", &speedTextSize );
			sscanf( speedTextOps->at(2), "%i", &speedTextX );
			sscanf( speedTextOps->at(3), "%i", &speedTextY );
		}
	}
	std::vector<char*>* courseOps;
	char courseLayer[7];
	float courseMax;
	float courseScale;
	std::vector<char*>* courseTextOps;
	char courseText[128] = {0};
	int courseTextSize = 0;
	int courseTextX = 0;
	int courseTextY = 0;
	if( ops.isSet( OPT_COURSE ) )
	{
		courseOps = splitArg( ops.getArg( OPT_COURSE ) );
		if( 3!=courseOps->size() )
			pError( "wrong argument count for option: --course" );
		if( 0 == strcmp( "d", courseOps->at(0) ) )
			strcpy( courseLayer, defaultLayer );
		else
			strcpy( courseLayer, courseOps->at(0) );
		sscanf( courseOps->at(1), "%f", &courseMax );
		sscanf( courseOps->at(2), "%f", &courseScale );
		
		// additional options for course
		if( ops.isSet( OPT_COURSETEXT ) )
		{
			courseTextOps = splitArg( ops.getArg( OPT_COURSETEXT ) );
			if( 4!=courseTextOps->size() )
				pError( "wrong argument count for option: --course" );
			strcpy( courseText, courseTextOps->at(0) );
			sscanf( courseTextOps->at(1), "%i", &courseTextSize );
			sscanf( courseTextOps->at(2), "%i", &courseTextX );
			sscanf( courseTextOps->at(3), "%i", &courseTextY );
		}
	}
	
	//init_image();
	//Gi.init( ops.isSet( OPT_SPEED )
	//        ,ops.isSet( OPT_COURSE ) );
	GI_ERR ierr = Gi.init( ops.isSet( OPT_SPEED )
	                      ,ops.isSet( OPT_COURSE ) );
	if( ierr != GI_ERR_SUCCESS )
	{
		std::cerr << "output folder './output/";
		switch( ierr )
		{
			case GI_ERR_OUTPUT:    std::cerr << "";       break;
			case GI_ERR_OUTSPEED:  std::cerr << "speed";  break;
			case GI_ERR_OUTCOURSE: std::cerr << "course"; break;
		}
		std::cerr << "' does not exist." << std::endl;
		
		return EXIT_FAILURE;
	}
	if( ops.isSet( OPT_FONT ) && !Gi.setFontTTF( ops.getArg( OPT_FONT ) ) )
		pError( "fontfile not found or readable" );
	
	
	// Main programm
	
	
	GPX_TOKEN state = GPX_IGNORE; // ignore lines until we hit a known one
	char lat[32] = { '0' };
	char lon[32] = { '0' };
	char time[32] = { '0' };
	char course[32] = { '0' };
	char speed[32] = { '0' };
	unsigned int nr = 0; // image counter
	char out[128] = "";
	char imgNr[8] = "0";
	char imgName[32];

	// now read relevant
	while( Gpx.getLine( buffer, BUF ) )
	{
		// skip preceding spaces
		unsigned int x = 0;
		while( 0==strncmp( &buffer[x], " ", 1 ) )
			x++;
		// compare line
		if(      ops.isSet( OPT_COORDS ) && 0==strncmp( &buffer[x], "<trkpt ", 7) ) state = GPX_TRKPT;
		else if( ops.isSet( OPT_TIME )   && 0==strncmp( &buffer[x], "<time",   5) ) state = GPX_TIME;
		else if( ops.isSet( OPT_COURSE ) && 0==strncmp( &buffer[x], "<course", 7) ) state = GPX_COURSE;
		else if( ops.isSet( OPT_SPEED )  && 0==strncmp( &buffer[x], "<speed",  6) ) state = GPX_SPEED;
		else if(                            0==strncmp( &buffer[x], "</trkpt", 7) ) state = GPX_END;
		else                                                                        state = GPX_IGNORE;
		
		switch( state )
		{
			case GPX_TRKPT:
				dom_get_prop( buffer, lat, "lat" );
				dom_get_prop( buffer, lon, "lon" );
				strcat( out, " la=" );
				strcat( out, lat );
				strcat( out, " lo=" );
				strcat( out, lon );
				break;
				
			case GPX_TIME:
				dom_get_value( buffer, time );
				strcat( out, " ti=" );
				strcat( out, time );
				break;
				
			case GPX_COURSE:
				dom_get_value( buffer, course );
				strcat( out, " cu=" );
				strcat( out, course );
				strcpy( imgName, "./output/course/course_" );
				strcat( imgName, imgNr );
				strcat( imgName, ".png" );
				Gi.courseImage();
				Gi.value( course, courseMax, courseScale );
				Gi.layer( courseLayer );
				if( ops.isSet( OPT_COURSETEXT ) )
					Gi.text( (0==strcmp(courseText,"d") ?NULL :courseText),
					         courseTextSize, courseTextX, courseTextY );
				Gi.save( imgName );
				break;
				
			case GPX_SPEED:
				dom_get_value( buffer, speed );
				strcat( out, " sp=" );
				strcat( out, speed );
				strcpy( imgName, "./output/speed/speed_" );
				strcat( imgName, imgNr );
				strcat( imgName, ".png" );
				Gi.speedImage();
				Gi.value( speed, speedMax, speedScale );
				Gi.layer( speedLayer );
				if( ops.isSet( OPT_SPEEDTEXT ) )
					Gi.text( (0==strcmp(speedText,"d") ?NULL :speedText),
					         speedTextSize, speedTextX, speedTextY,
					         (ops.isSet( OPT_FONTCOLOR )) ?ops.getArg( OPT_FONTCOLOR ) :NULL );
				Gi.save( imgName );
				break;
				
			case GPX_END:
				std::cout << nr << ":" << out << std::endl;
				
				// increase image-counter / sequenz number
				++nr;
				sprintf( imgNr, "%d", nr );
				
				// reset values to zero (char)
				out[0] = '\0';
				strcpy( lat, "0" );
				strcpy( lon, "0" );
				strcpy( time, "0" );
				strcpy( course, "0" );
				strcpy( speed, "0" );
				break;
				
			case GPX_IGNORE:
			default:
				continue;
				break;
		} // switch( state )
	} // while( getline )

	return EXIT_SUCCESS;
}




std::vector<char*>* splitArg( const char* arg )
{
	std::vector<char*>* v = new std::vector<char*>;
	char* token;
	
	token = strtok( (char*)arg, "," );
	while( token != NULL )
	{
		v->push_back( token );
		token = strtok ( NULL,  "," );
	}
	
	return v;
}




/**
 * There must be a better way... but for the first
 */
void pUsage( void )
{
	std::cout << ""
	"Usage: \n"
	"    " << "gpx2imgseq" << " [options] filename.gpx\n"
	"\n"
	"  All images are restricted to a size of 256x256 pixel.\n"
	"  All rotations have their gravity in the center of the image x128 y128.\n"
	"\n"
	"options:\n"
	"  --speed=<layerDef>,<maxSpeed>,<maxDegree>\n"
	"      Creates Speed-Images, rotation depends on GPX-Speed-Value.\n"
	"      \n"
	"      <layerDef> describes the order in which the layer are drawn into the\n"
	"      output image. It consists of 'b','a','g' OR just 'd'.\n"
	"      'b/a/g' represents Background, Arrow and Gauge. The boolean behind each\n"
	"      character determines whether the image is rotated or not.\n"
	"      'd' stands for default and is equivalent to 'b0a1g0' which means layer-\n"
	"      order: 1.Background, 2.Arrow and on top 3.Gauge, Arrow is rotated by\n"
	"      GPX-Speed.\n"
	"      You can use one or more layer. i.e. 'g1' or 'g1b0a1g0'\n"
	"      \n"
	"      <maxSpeed> specifies the desired maximal speed. It depents on the used\n"
	"      graphics. If the graphic shows a scale from 0-50, 50 is the maxSpeed.\n"
	"      If GPX-Speed exceeds <maxSpeed>, overrotation will happen.\n"
	"      \n"
	"      <maxDegree> specifies the maximal rotation on maximal speed. If the\n"
	"      scale(0-50) on the graphic is a half circle then max degree is 180.\n"
	"      \n"
	"      `--speed=d,50,180` Layerdef=b0a1g0, maxSpeed=50 & maxDegree=180\n"
	"          1.Background, 2.Arrow with rotation and on top 3.Gauge.\n"
	"          The rotation by maxSpeed of 50 will be 180 degree.\n"
	"      `--speed=a1g0,120,90` Layerdef=a1g0, maxSpeed=120 & maxDegree=90\n"
	"          1.Arrow with rotation and on top 2.Gauge. At speed of 120 the\n"
	"          arrowlayer will be rotated by 90 degree.\n"
	"      \n"
	"  --speed_text=<txt>,<fontSize>,<fontPosX>,<fontPosY>\n"
	"      Generate text on Speed-Image.\n"
	"      \n"
	"      <txt> Could be a freetext or just 'd'.\n"
	"      If 'd' then the GPX-Speed round to one decimal place is drawn onto image.\n"
	"      \n"
	"      <fontSize> The size in which the text is drawn.\n"
	"      \n"
	"      <fontPosX>,\n"
	"      <fontPosY> The X and Y cooordinates within the image. The text is\n"
	"         centered on that position\n"
	"      \n"
	"      `--speed_text=d,55,128,100` Draws the GPX-Speed rounded to one decimal\n"
	"          place in the center, slightly above the middle line.\n"
	"      \n"
	"  --course=<layerDef>,<maxCourse>,<maxDegree>\n"
	"      Creates Course-Images, rotation depends on GPX-Course-Value.\n"
	"      see `--speed`\n"
	"      In most cases <maxCourse> and <maxDegree> would be both 360, I guess.\n"
	"      \n"
	"      `--course=d,360,360` Layerdef=b0g0a1, maxCourse=360 & maxDegree=360\n"
	"          1.Background, 2.Arrow with rotation and on top 3.Gauge.\n"
	"          The GPX-Course in degree is fully represented by full rotation.\n"
	"      \n"
	"  --course_text=<txt>,<fontSize>,<fontPosX>,<fontPosY>\n"
	"      Generate Text on Course-Image.\n"
	"      see `--speed_text`\n"
	"      \n"
	"  --font=<ttfPath>\n"
	"      Specifies the TrueTypeFont-File to use when text is drawn onto the image.\n"
	"      The default is '/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf'\n"
	"      Absolute path is required.\n"
	"      \n"
	"  --font_color=<color>\n"
	"      The colordefinition to use for text that is drawn onto the image.\n"
	"      \n"
	"      <color> can be in HEX-Notation i.e. #ff0000 for red and #000000 for black.\n"
	"      It could also be a name like 'LawnGreen' supported by ImageMagick.\n"
	"      https://imagemagick.org/script/color.php#color_names\n"
	"\n"
	"" << std::endl;
	
	// FIXME if we call --help  ...thats not an failure.
	exit( EXIT_FAILURE );
}




void pError( const char* msg )
{
	std::cerr << "STOP: " << msg << std::endl;
	exit( EXIT_FAILURE );
}




// EOF
