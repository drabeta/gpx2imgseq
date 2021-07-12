// vim: set ts=4:
// http://www.mario-konrad.ch/blog/programming/getopt.html
#include <iostream>
#include <cstdlib>

#include "option.hpp"




Option::Option() {}
Option::~Option(){}




bool Option::setShort( char shortOpt[] )
{
	//short_options = shortOpt;
	//return true;
	return NULL;
}




bool Option::setLong( struct option longOpt[] )
{
	//long_options = longOpt;
	//return true;
	return NULL;
}




bool Option::load( int argc, char* argv[] )
{

	/*
		ähm... right now all options are set and handled in this method
		by handling i mean "converted" into a bitset
		i thougt about setter-methods to set short and long options befor read them.
		but with this switch here, that makes no sense...
		for the first, let it here.
	*/
	char short_options[] = ":sctg"; // preceeding : means we handle errors
	static const struct option long_options[] =
	{
		{ "speed" , no_argument, 0, 's' },
		{ "course", no_argument, 0, 'c' },
		{ "time"  , no_argument, 0, 't' },
		{ "coords", no_argument, 0, 'g' },
		0
	};

	int index = -1;
	int result;
	while( -1!=(result = getopt_long( argc, argv,
		                              short_options,
		                              long_options, &index ) ) )
	{
		struct option * opt = nullptr;
		switch( result )
		{
			case '?': /* unknown parameter */
				opt = (struct option *)&(long_options[index]);
				std::cerr << "unknown parameter: " << argv[optind-1] << std::endl;
				bits |= OPT_ERROR;
				break;
			case ':': /* missing argument of a parameter */
				std::cerr << "missing argument" << std::endl;
				bits |= OPT_ERROR;
				break;
			case 's': /* same as index==0 */
				//std::cout << "'s'/'speed' was specified." << std::endl;
				bits |= OPT_SPEED;
				break;
			case 'c': /* same as index==0 */
				//std::cout << "'c'/'course' was specified." << std::endl;
				bits |= OPT_COURSE;
				break;
			case 't': /* same as index==0 */
				//std::cout << "'t'/'time' was specified." << std::endl;
				bits |= OPT_TIME;
				break;
			case 'g': /* same as index==0 */
				//std::cout << "'g'/'coord' was specified." << std::endl;
				bits |= OPT_COORDS;
				break;
			case 0: /* all parameter that do not */
			        /* appear in the optstring */
				opt = (struct option *)&(long_options[index]);
				printf( "'%s' was specified.", opt->name );
				if( opt->has_arg==required_argument )
					printf( "Arg: <%s>", optarg );
				printf( "\n" );
				break;
			default: /* unknown */
				break;
		} // switch( result )
	} // while( ... getopt_long() ... )
	
	if( bits & OPT_ERROR )
		return false;
	
	return true;
}




int Option::get()
{
	return bits;
}




bool Option::isSet( opts option )
{
	std::cout << "b" << bits << std::endl;
	std::cout << "o"<<option << std::endl;
	if( bits & option )
		return true;
	return false;
}




// EOF
