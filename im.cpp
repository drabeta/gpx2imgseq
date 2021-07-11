// vim: set ts=4:
#include <iostream>
#include <Magick++.h>

using namespace Magick;

int main(int argc,char **argv)
{
    InitializeMagick(*argv);
	
    //Image i( "256x256", "transparent" );
	Color t( 0x0,0x0,0x0,0xFFFF );
	
	Image b;
		b.size( "256x256" );
		b.backgroundColor( t );
		b.read( "images/course_backg.svg" );

	Image c;
		c.size( "256x256" );
		c.backgroundColor( t );
		c.read( "images/course_gauge.svg" );

	Image n;
		n.size( "256x256" );
		n.backgroundColor( t );
		n.read( "images/course_arrow.svg" );
		n.rotate( 30.0 );
		n.crop( Geometry( 256,256,
//		                  (a.columns()-256)/2, (a.rows()-256)/2)
0,0) );


	Image i = b;

	i.composite( c ,0,0, OverCompositeOp );
	i.composite( n, 0,0, OverCompositeOp );
	i.write( "aa_course.png" );
	//g.composite( a, ((a.columns()-256)/2)*-1, ((a.rows()-256)/2)*-1, OverCompositeOp );
//	a.composite( g, 0,0, OverCompositeOp );
	
	//a.font( "-*-terminal-bold-*-*-*-150-*-*-*-*-*-*-*" );
//	a.font( "@/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf" );
//	a.fontPointsize( 60 );
//	a.annotate( "31", Geometry( 1,1, 128, 95 ), CenterGravity );
	
//	a.write( "aaaa.png" );
    
	return 0;
}
