// vim: set ts=4:
#ifndef _OPTION_HPP_
#define _OPTION_HPP_

#include <cstdio>
#include <getopt.h>


enum opts
{
	OPT_SPEED=1,
	OPT_COURSE=2,
	OPT_TIME=4,
	OPT_COORDS=8,
	OPT_ERROR=16
};


class Option
{
	private:
	char* short_options;
	struct option* long_options;
	unsigned int bits = 0x0;
	//unsigned int optbit = 0x0;
	
	public:
	Option();
	~Option();
	bool setShort( char[] );
	bool setLong( struct option[] );
	bool load( int argc, char* argv[] );
	int get();
	bool isSet( opts option );
};


#endif
// EOF
