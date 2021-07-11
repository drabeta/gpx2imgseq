// vim: set ts=4:
#ifndef _FILE_HPP_
#define _FILE_HPP_


#include <fstream>



class File
{
	private:
	std::fstream file;
	
	
	public:
	//File();
	~File();
	bool open( const char filename[] );
	bool getLine( char buffer[], int size );
};


#endif
// EOF
