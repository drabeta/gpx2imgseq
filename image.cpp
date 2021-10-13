// vim: set ts=4:
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <Magick++.h>

#include "image.hpp"




GpxImage::~GpxImage()
{}




GpxImage::GpxImage()
{
	strcpy( mFont,      "@/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf" );
	strcpy( mFontColor, "#000000" );
}




GI_ERR GpxImage::init( bool speed, bool course )
{
	Magick::InitializeMagick( nullptr );
	mTransp = Magick::Color( 0x0, 0x0, 0x0, 0xFFFF );
	
	// Check output folder
	struct stat statFb;
	if( -1 == stat( "./output", &statFb ) )
	{
		if( -1 == mkdir( "./output", 0640 ) )
			return GI_ERR_OUTPUT;
	}
	
	if( speed )
	{
		// Check output subfolder
		if( -1 == stat( "./output/speed", &statFb ) )
		{
			if( -1 == mkdir( "./output/speed", 0640 ) )
				return GI_ERR_OUTSPEED;
		}
		
		mSpeedBackg.backgroundColor( mTransp );
		mSpeedBackg.read( "images/speed_backg.svg" );
		// FIXME reads 256x256-image as 259x259
		// i have to resize back to 256x256
		mSpeedBackg.resize( mSize );
		mSpeedGauge.backgroundColor( mTransp );
		mSpeedGauge.read( "images/speed_gauge.svg" );
		mSpeedGauge.resize( mSize );
		mSpeedArrow.backgroundColor( mTransp );
		mSpeedArrow.read( "images/speed_arrow.svg" );
		mSpeedArrow.resize( mSize );
	}
	if( course )
	{
		if( -1 == stat( "./output/course", &statFb ) )
		{
			if( -1 == mkdir( "./output/course", 0640 ) )
				return GI_ERR_OUTCOURSE;
		}
		mCourseBackg.backgroundColor( mTransp );
		mCourseBackg.read( "images/course_backg.svg" );
		mCourseBackg.resize( mSize );
		mCourseGauge.backgroundColor( mTransp );
		mCourseGauge.read( "images/course_gauge.svg" );
		mCourseGauge.resize( mSize );
		mCourseArrow.backgroundColor( mTransp );
		mCourseArrow.read( "images/course_arrow.svg" );
		mCourseArrow.resize( mSize );
	}
	
	return GI_ERR_SUCCESS;
}




void GpxImage::src( Magick::Image& refBackg, Magick::Image& refGauge, Magick::Image& refArrow )
{
	mpB = &refBackg;
	mpG = &refGauge;
	mpA = &refArrow;
}




void GpxImage::value( char* value, float max, float scaleSize )
{
	sscanf( value, "%f", &mValue );
	mValueMax = max;
	mValueScale = scaleSize;
	
	mValueConv = mValue * mValueFac;                          // conv m/s into kmh/knt/mph
	mValueDeg  = mValueConv * ((float)mValueScale/mValueMax); // conv into deg on scale
}




void GpxImage::setKmh()
{
	mValueFac = 3.6;
}




void GpxImage::setMph()
{
	mValueFac = 2.23694;
}




void GpxImage::setKnt()
{
	mValueFac = 1.94384;
}




void GpxImage::setRaw()
{
	mValueFac = 1.0;
}




GI_ERR GpxImage::layer( char layerDef[] )
{
	int layerCount = strlen( layerDef );
	int curLayer = 0;
	
	while( curLayer<layerCount )
	{
		switch( layerDef[curLayer] )
		{
			case 'b': mT = *mpB; break;
			case 'g': mT = *mpG; break;
			case 'a': mT = *mpA; break;
			default:
				return GI_ERR_LAYER;
		}
		if( layerDef[curLayer+1]==49 ) // int(49)=char(1)->"true"  48=0"false"
		{
			mT.rotate( mValueDeg );
			mT.crop( Magick::Geometry( 256,256, 0, 0 ) );
		}
		
		// FIXME I had trouble when "compositing" an refImage into new/empty mI
		// no pixel or pixel not found...   I may have to initialise mI an other way
		if( curLayer==0 ) // only first layer
			mI = mT;
		else
			mI.composite( mT, 0, 0, Magick::OverCompositeOp );
		
		curLayer+=2;
	}
	
	return GI_ERR_SUCCESS;
}




GI_ERR GpxImage::setFontTTF( const char fontFile[] )
{
	struct stat fb;
	if( stat( fontFile, &fb ) != 0 )
		return GI_ERR_FONT;
	
	strcpy( mFont, "@" );
	strcat( mFont, fontFile );
	
	return GI_ERR_SUCCESS;
}




GI_ERR GpxImage::setFontColor( const char fontColor[] )
{
	strcpy( mFontColor, fontColor );
	
	return GI_ERR_SUCCESS;
}




void GpxImage::text( char text[], int fontSize, int x, int y, const char color[] )
{
	if( text == NULL )
		sprintf( mText, "%.1f", mValueConv );
	else
		strcpy( mText, text );
	mTextSize = fontSize;
	mTextX = x;
	mTextY = y;
	
	mI.font( mFont );
	mI.fontPointsize( mTextSize );
	mI.fillColor( (color!=NULL) ?color :mFontColor );
	mI.annotate( mText, Magick::Geometry( 1, 1, mTextX, mTextY ), Magick::CenterGravity );
}




void GpxImage::crop( char cropDef[] )
{}




GI_ERR GpxImage::save( char filename[] )
{
	mI.write( filename );
	return GI_ERR_SUCCESS;
}




// can I avoid this methodes
void GpxImage::speedImage()
{
	mpB = &mSpeedBackg;
	mpG = &mSpeedGauge;
	mpA = &mSpeedArrow;
	setKmh();
}




void GpxImage::courseImage()
{
	mpB = &mCourseBackg;
	mpG = &mCourseGauge;
	mpA = &mCourseArrow;
	setRaw();
}




/* DEPRECATED / backup * /


Magick::Image speedGauge;
Magick::Image speedArrow;
Magick::Image speedBackg;
Magick::Image courseGauge;
Magick::Image courseArrow;
Magick::Image courseBackg;
Magick::Image image;
Magick::Image arrow;


void init_image()
{
	Magick::InitializeMagick( nullptr );
	Magick::Color transp( 0x0, 0x0, 0x0, 0xFFFF );
	speedBackg.backgroundColor( transp );
	speedBackg.size( "256x256" );
	speedBackg.read( "images/speed_backg.svg" );
	speedGauge.backgroundColor( transp );
	speedGauge.size( "256x256" );
	speedGauge.read( "images/speed_gauge.svg" );
	speedArrow.backgroundColor( transp );
	speedArrow.size( "256x256" );
	speedArrow.read( "images/speed_arrow.svg" );
	courseBackg.backgroundColor( transp );
	courseBackg.size( "256x256" );
	courseBackg.read( "images/course_backg.svg" );
	courseGauge.backgroundColor( transp );
	courseGauge.size( "256x256" );
	courseGauge.read( "images/course_gauge.svg" );
	courseArrow.backgroundColor( transp );
	courseArrow.size( "256x256" );
	courseArrow.read( "images/course_arrow.svg" );
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
/* */




// EOF
