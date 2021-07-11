// vim: set ts=4:
#include <cstring>

#include "parse.hpp"




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




// EOF
