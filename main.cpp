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

#include "image.hpp"

#define BUF 256


enum GPX_TOKEN { GPX_TRKPT, GPX_TIME, GPX_COURSE, GPX_SPEED, GPX_END, GPX_IGNORE };


void pError( const char* msg );
void dom_get_prop( char* buffer, char* var, const char* prop );
void dom_get_value( char* buffer, char* var );


std::fstream file;
char buffer[BUF];
enum GPX_TOKEN state;



int main( int argc, char* argv[] )
{
	init_image();

	file.open( "input.gpx", std::fstream::in );
	if( !file.is_open() )
		pError( "Can not open GPX-File!" );

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
		file.ignore( 1 , '\n' );
		
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
	} // while( file.getline() )

	file.close();
	return EXIT_SUCCESS;
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
	// prepare searchvalue by adding '="' to propertyname to prevent missmatch 
	int len = strlen( prop );
	char search[32] = { '\0' };
		strcpy( search, prop );
		search[len]   = '=';    // add =
		search[len+1] = '"';    // add "
		search[len+2] = '\0';   // finish with NULL
	len+=2;                     // adjust length-value

	// start- and endpositions of propertyvalue
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




// EOF
