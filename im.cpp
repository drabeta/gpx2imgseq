// vim: set ts=4:
#include <iostream>
#include "image.hpp"


void p( char* hint )
{
	std::cout << "HINT: " << hint << std::endl;
}



GpxImage gi;

int main( int argc,char **argv )
{
	p( "init" );
	gi.init( true, true );
	p( "speedImage" );
	gi.speedImage();
	p( "kmh" );
	gi.setKmh();
	p( "value" );
	gi.value( "2.0234", 50, 180 );
	p( "layer" );
	gi.layer( "b0a1g0" );
	//gi.layer( "b0" );
	p( "font" );
	gi.setFontTTF( "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf" );
	p( "text" );
	gi.text( NULL, 55, 128, 100 );
	p( "save" );
	gi.save( "./aaaa.png" );
	
	
p( "FERTIG" );
		
	
	
	
	return 0;
}
