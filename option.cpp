// vim: set ts=4:
// http://www.mario-konrad.ch/blog/programming/getopt.html
#include <iostream>
#include <cstdlib>
#include <cstring>

#include "option.hpp"




Option::~Option()
{}




Option::Option()
{
	// iterates through `enum opts` and adds empty values into map arguments
	// opts are defined for use als bit-setter 0x1 0x2 0x4 0x8. bitshift by one
	// OPT_ARGSEND defines the end of options WITH arguments, OPT_OPTEND the end
	// of the enum opts
	for( int i=1; i<OPT_ARGSEND; i<<=1 )
		arguments.emplace( i, "" );
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
	char short_optionsss[] = ":a"; // preceeding : means we handle errors
	                               // "W" is reserved by POSIX.2
	static const struct option long_optionsss[] =
	{
		// all options need an enum opts
		{ "speed" ,      required_argument, nullptr, OPT_SPEED      },
		{ "speed_text" , required_argument, nullptr, OPT_SPEEDTEXT  },
		{ "course",      required_argument, nullptr, OPT_COURSE     },
		{ "course_text", required_argument, nullptr, OPT_COURSETEXT },
		{ "font",        required_argument, nullptr, OPT_FONT       },
		{ "font_color",  required_argument, nullptr, OPT_FONTCOLOR  },
		{ "time"  ,      no_argument,       nullptr, OPT_TIME       },
		{ "coords",      no_argument,       nullptr, OPT_COORDS     },
		NULL
	};

	int index = -1;
	int current_option;
	while( -1!=(current_option = getopt_long( argc, argv,
		                                      short_optionsss,
		                                      long_optionsss, &index ) ) )
	{
		switch( current_option )
		{
			case '?': /* unknown parameter */
				std::cerr << "unknown parameter: " << argv[optind-1] << std::endl;
				bits |= OPT_ERROR;
				break;
			case ':': /* missing argument of a parameter */
				std::cerr << "missing argument for: " << argv[optind-1] << std::endl;
				bits |= OPT_ERROR;
				break;
			default: // case for argumented options
				bits |= current_option;
				arguments[current_option] = optarg; // or better strcpy !?
				break;
/* * /
			struct option * opt = nullptr;
			case 'x': // case for non-argumented options
				opt = (struct option *)&(long_optionsss[index]);
				printf( "'%s' was specified.", opt->name );
				if( opt->has_arg==required_argument )
					printf( "Arg: <%s>", optarg );
				printf( "\n" );
				break;
/* */
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
	if( bits & option )
		return true;
	return false;
}




const char* Option::getArg( opts option )
{
	return arguments.find(option)->second;
}




// EOF
