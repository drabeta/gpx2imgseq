// vim: set ts=4:
#include <cstring>
#include <Magick++.h>

#include "image.hpp"


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




// EOF
