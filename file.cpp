// vim: set ts=4:

#include "file.hpp"




File::~File()
{
	file.close();
}




bool File::open( const char filename[] )
{
	file.open( filename, std::fstream::in );
	if( !file.is_open() )
		return false;
	
	return true;
}




bool File::getLine( char buffer[], int size )
{
	// my file contains \r\n for line endings
	// if i read until the \n i got problems on string comparson
	// read until \r and then ignore until \n
	// TODO: let the user determine the line ending
	// line ending is depending on file generating system
	
	file.getline( buffer, size, '\r' );
	if( !file.good() )
		return false;
		
	file.ignore( 1 , '\n' );
	
	return true;
}




// EOF
