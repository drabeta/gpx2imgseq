// vim: set ts=4:
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

#include <Magick++.h>

#define BUF 256


enum GPX_TOKEN { GPX_TRKPT, GPX_TIME, GPX_COURSE, GPX_SPEED, GPX_END, GPX_IGNORE };


void pError( const char* msg );
void dom_get_prop( char* buffer, char* var, const char* prop );
void dom_get_value( char* buffer, char* var );
void gen_image( unsigned int nr, float speed, float course );


std::fstream file;
char buffer[BUF];
enum GPX_TOKEN state;
Magick::Image speedGauge;
Magick::Image speedArrow;
Magick::Image speedBackg;
Magick::Image courseGauge;
Magick::Image courseArrow;
Magick::Image courseBackg;
Magick::Image image;
Magick::Image arrow;



int main( int argc, char* argv[] )
{
	char* dummy = nullptr;
	Magick::InitializeMagick( dummy );
	Magick::Color transp( 0x0, 0x0, 0x0, 0xFFFF );
	speedGauge.backgroundColor( transp );
	speedGauge.size( "256x256" );
	speedGauge.read( "images/speed_gauge.svg" );
	speedArrow.backgroundColor( transp );
	speedArrow.size( "256x256" );
	speedArrow.read( "images/speed_arrow.svg" );
	speedBackg.backgroundColor( transp );
	speedBackg.size( "256x256" );
	speedBackg.read( "images/speed_backg.svg" );
	courseGauge.backgroundColor( transp );
	courseGauge.size( "256x256" );
	courseGauge.read( "images/course_gauge.svg" );
	courseBackg.backgroundColor( transp );
	courseBackg.size( "256x256" );
	courseBackg.read( "images/course_backg.svg" );
	courseArrow.backgroundColor( transp );
	courseArrow.size( "256x256" );
	courseArrow.read( "images/course_arrow.svg" );

	file.open( "6-20-14-29-15_15_1625505170812.gpx", std::fstream::in );
	if( !file.is_open() )
		pError( "GPX-Datei konnte nicht geöffnet werden" );

	// skip first 5 lines / ignore them
	file.getline( buffer, BUF );
	file.getline( buffer, BUF );
	file.getline( buffer, BUF );
	file.getline( buffer, BUF );
	file.getline( buffer, BUF );
	state=GPX_TRKPT;

	char la[32] = { '0' };
	char lo[32] = { '0' };
	char ti[32] = { '0' };
	char cu[32] = { '0' };
	char sp[32] = { '0' };
	unsigned int nr = 0; // image counter
	// now read relevant
	while( file.getline( buffer, BUF, '\r' ) )
	{
		//std::cout << buffer << std::endl;
		file.ignore( 1 , '\n');
		
		// skip preceding spaces
		unsigned int x = 0;
		while( 0==strncmp( &buffer[x], " ", 1) )
			x++;
		// compare line
		if(      0==strncmp(&buffer[x],"<trkpt ", 7) )    state = GPX_TRKPT;
		else if( 0==strncmp(&buffer[x],"<time",   5) )    state = GPX_TIME;
		else if( 0==strncmp(&buffer[x],"<course", 7) )    state = GPX_COURSE;
		else if( 0==strncmp(&buffer[x],"<speed",  6) )    state = GPX_SPEED;
		else if( 0==strncmp(&buffer[x],"</trkpt", 7) )    state = GPX_END;
		else                                              state = GPX_IGNORE;

		float fSpeed = 0.0;
		float fCourse = 0.0;
		switch( state )
		{
			case GPX_TRKPT:
				dom_get_prop( buffer, la, "lat" ); 
				dom_get_prop( buffer, lo, "lon" );
				break;
			case GPX_TIME:   dom_get_value( buffer, ti ); break;
			case GPX_COURSE: dom_get_value( buffer, cu ); break;
			case GPX_SPEED:  dom_get_value( buffer, sp ); break;
			case GPX_END:
				std::cout << nr << ":"
				          << " la=" << la 
				          << " lo=" << lo
				          //<< " time=" << ti
				          << " cu=" << cu
				          << " sp=" << sp
				          << std::endl;
				sscanf( sp, "%f", &fSpeed );
				sscanf( cu, "%f", &fCourse );
				// generate images for speed and course
				gen_image( nr++, fSpeed, fCourse );
				break;
			case GPX_IGNORE:
			default:
				continue;
				break;
		} // switch( state )
	} // while( file.getline() )

	file.close();
	return EXIT_SUCCESS;
}



void gen_image( unsigned int nr, float speed, float course )
{
	char imgNr[8] = { '\0' };
		sprintf( imgNr, "%d", nr );
	char imgNameSpeed[32] = { '.','/','o','u','t','p','u','t','/','s','p','e','e','d','_', '\0' };
		strcat( &imgNameSpeed[15], imgNr );
		strcat( &imgNameSpeed[15+strlen(imgNr)], ".png" );
	char imgNameCourse[32] = { '.','/','o','u','t','p','u','t','/','c','o','u','r','s','e','_', '\0' };
		strcat( &imgNameCourse[16], imgNr );
		strcat( &imgNameCourse[16+strlen(imgNr)], ".png" );
	float deg = ((float)180/50)*(speed*3.6);
	char kmh[5];
		sprintf( kmh, "%.1f", speed*3.6 );
		kmh[4] = '\0';

	// SPEED
	arrow = speedArrow;
		arrow.rotate( deg );
		arrow.crop( Magick::Geometry( 256,256, 0,0 ) );
	image = speedBackg;
		image.size( "256x256" );
		image.composite( arrow, 0,0, Magick::OverCompositeOp );
		image.composite( speedGauge, 0,0, Magick::OverCompositeOp );
		image.font( "@/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf" );
		image.fontPointsize( 55 );
		image.annotate( kmh, Magick::Geometry( 1,1, 128, 100 ), Magick::CenterGravity );
		image.write( imgNameSpeed );

	// COURSE
	arrow = courseArrow;
		arrow.rotate( course );
		arrow.crop( Magick::Geometry( 256,256, 0,0 ) );
	image = courseBackg;
		image.size( "256x256" );
		image.composite( courseGauge, 0,0, Magick::OverCompositeOp );
		image.composite( arrow, 0,0, Magick::OverCompositeOp );
		image.write( imgNameCourse );
}



void dom_get_value( char* buffer, char* var )
{
	char* pos = nullptr;
	int startPos = 0;
	int endPos = 0;
	int len = 0;
	
	pos = strstr( buffer, ">" );
	startPos = pos-buffer+1;

	pos = strstr( &buffer[startPos], "<" );
	endPos = pos-buffer;

	len = endPos-startPos;

	strncpy( var, &buffer[startPos], len );
	var[len] = '\0';
}



void dom_get_prop( char* buffer, char* var, const char* prop )
{
	// prepare searchvalue
	char search[32] = { '\0' };
	int len = 0;
	// adding '="' to propertyname to prevent missmatch 
	strcpy( search, prop );
	len = strlen( prop );
	search[len]   = '=';    // add =
	search[len+1] = '"';    // add "
	search[len+2] = '\0';   // finish with NULL
	len+=2;                 // adjust length-value

	// start and end positions of propertyvalue
	char* pos = nullptr;
	int startPos = 0;
	int endPos = 0;
	// find start pos
	pos = strstr( buffer, search );
	startPos = pos-buffer +len;
	// find end pos
	pos = strstr( &buffer[startPos], "\"" );
	endPos = pos-buffer;

	// calculate length of value
	len = endPos-startPos;
	// copy value to ref-var
	strncpy( var, &buffer[startPos], len );
	var[len]='\0';

	//std::cout << ">" << var << " - " << prop << " @ " << startPos << std::endl;
}



void pError( const char* msg )
{
	std::cerr << "STOP: " << msg << std::endl;
	file.close();
	exit( EXIT_FAILURE );
}

