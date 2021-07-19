// vim: set ts=4:
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <vector>

#include "option.hpp"


Option O;



std::vector<char*>* courseOps;

std::vector<char*>* splitOps( const char* );



int main( int argc, char* argv[] )
{
	if( !O.load( argc, argv)  )
	{
		std::cout << "errors: abort " << O.get() << std::endl;
		return EXIT_FAILURE;
	}
	if( optind+1 != argc )
	{
		std::cerr << "missing filename" << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "inputfile: " << argv[optind++] << std::endl;
		
	// MAIN-PROGRAM
	
	std::cout << "BiTS: " << O.get() << std::endl;
	
	if( O.isSet( OPT_SPEED )  )
	{
		std::cout << "speed";
		
		const char* a = O.getArg( OPT_SPEED );
		std::cout << ":" << a << " ";
		
		// b0,g0,a1
		// b0g0a1
		std::unordered_map< int, const char* > layerImages;
		layerImages.emplace( 'b', "backg" );
		layerImages.emplace( 'g', "gauge" );
		layerImages.emplace( 'a', "arrow" );
		
		int layerCount = strlen( a ) / 2;
		for( int i=0; i<layerCount; ++i )
		{
			std::cout << layerImages.find( a[i*2] )->second;
			if( (int)a[i*2 +1]==49 ) // int(49) = char(1) = true
				std::cout << "(R)";
			std::cout << " ";
		}
		
		std::cout << std::endl;
	}
	if( O.isSet( OPT_COURSE ) )
	{
		std::cout << "course";
		
		const char* a = O.getArg( OPT_COURSE );
		std::cout << ":" << a << " ";
		std::vector<char*>* v = splitOps( O.getArg( OPT_COURSE ) );
		std::cout << v->at(0) << " "
		          << v->at(2) << std::endl;
		
		std::cout << std::endl;
	}
	if( O.isSet( OPT_TIME )   ) std::cout << "time " << std::endl;
	if( O.isSet( OPT_COORDS ) ) std::cout << "coords " << std::endl;
	
	return EXIT_SUCCESS;
}




std::vector<char*>* splitOps( const char* arg )
{
	std::vector<char*>* v = new std::vector<char*>;
	char* token;
	
	token = strtok( (char*)arg, "," );
	while( token != NULL )
	{
		v->push_back( token );
		token = strtok (NULL,  "," );
	}
	
	return v;
}







// EOF
