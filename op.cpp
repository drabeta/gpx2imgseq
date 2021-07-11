// vim: set ts=4:
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <getopt.h>



char short_options[] = ":sctg"; // preceeding : means we handle errors
static const struct option long_options[] =
{
	{ "speed" , no_argument, 0, 's' },
	{ "course", no_argument, 0, 'c' },
	{ "time"  , no_argument, 0, 't' },
	{ "coords", no_argument, 0, 'g' },
	0
};
unsigned int optbit = 0x0;
enum opts
{ 
	OPT_SPEED=1,
	OPT_COURSE=2,
	OPT_TIME=4,
	OPT_COORDS=8,
	OPT_ERROR=16
};
unsigned int read_options( int argc, char* argv[] );

int main( int argc, char* argv[] )
{
	optbit = read_options( argc, argv );
	if( optbit & OPT_ERROR  )
	{
		std::cout << "errors: abort " << optbit << std::endl;
		return EXIT_FAILURE;
	}
	if( optind+1 != argc )
	{
		std::cerr << "missing filename" << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "inputfile: " << argv[optind++] << std::endl;
		
	// MAIN-PROGRAM

	std::cout << "BiTS: " << optbit << std::endl;
	if( optbit & OPT_SPEED  ) std::cout << "speed ";
	if( optbit & OPT_COURSE ) std::cout << "cpurse ";
	if( optbit & OPT_TIME   ) std::cout << "time ";
	if( optbit & OPT_COORDS ) std::cout << "coords ";
	std::cout << std::endl;

	return EXIT_SUCCESS;
}


// http://www.mario-konrad.ch/blog/programming/getopt.html
unsigned int read_options( int argc, char* argv[] )
{
	unsigned int bits = 0x0;
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
	
	
	return bits;
}


// EOF
