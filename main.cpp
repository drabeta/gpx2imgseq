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
#include <cstdio>
#include <getopt.h>

#include "file.hpp"
#include "parse.hpp"
#include "image.hpp"
#include "option.hpp"

#define BUF 256


enum GPX_TOKEN { GPX_TRKPT, GPX_TIME, GPX_COURSE, GPX_SPEED, GPX_END, GPX_IGNORE };


void pError( const char* msg );


char buffer[BUF];
File Gpx;
Option ops;




int main( int argc, char* argv[] )
{
	if( !ops.load( argc, argv) )
		pError( "not properly configured" );
	
	if( optind+1 != argc )
		pError( "missing filename" );
	//std::cout << "inputfile: " << argv[optind] << std::endl;
	
	if( !Gpx.open( argv[optind] ) )
		pError( "Can not open GPX file" );
	pError( "DEBUG");
	
	init_image();
	
	
	// Main programm
	
	
	GPX_TOKEN state = GPX_IGNORE; // ignore lines until we hit a known one
	char la[32] = { '0' };
	char lo[32] = { '0' };
	char ti[32] = { '0' };
	char cu[32] = { '0' };
	char sp[32] = { '0' };
	unsigned int nr = 0; // image counter

	// now read relevant
	while( Gpx.getLine( buffer, BUF ) )
	{
		//std::cout << buffer << std::endl;
		
		// skip preceding spaces
		unsigned int x = 0;
		while( 0==strncmp( &buffer[x], " ", 1 ) )
			x++;
		// compare line
		if(      0==strncmp( &buffer[x], "<trkpt ", 7) )    state = GPX_TRKPT;
		else if( 0==strncmp( &buffer[x], "<time",   5) )    state = GPX_TIME;
		else if( 0==strncmp( &buffer[x], "<course", 7) )    state = GPX_COURSE;
		else if( 0==strncmp( &buffer[x], "<speed",  6) )    state = GPX_SPEED;
		else if( 0==strncmp( &buffer[x], "</trkpt", 7) )    state = GPX_END;
		else                                                state = GPX_IGNORE;

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
				// reset values to zero (char)
				strcpy( la, "0" );
				strcpy( lo, "0" );
				strcpy( ti, "0" );
				strcpy( cu, "0" );
				strcpy( sp, "0" );
				break;
			case GPX_IGNORE:
			default:
				continue;
				break;
		} // switch( state )
	} // while( getline )

	return EXIT_SUCCESS;
}




void pError( const char* msg )
{
	std::cerr << "STOP: " << msg << std::endl;
	exit( EXIT_FAILURE );
}




// EOF
