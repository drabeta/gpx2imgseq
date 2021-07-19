// vim: set ts=4:
#ifndef _OPTION_HPP_
#define _OPTION_HPP_

#include <cstdio>
#include <unordered_map>
#include <getopt.h>


/** enum guaranties size of int/4byte/32bit
 *  0b00000000000000000000000000000000
 *                                   ^-speed
 *                                  ^--speedtext
 *                                 ^---course
 *                                ^----coursetext
 *                               ^-----
 */
// note: if options exceeds 32 (int32)
// since C++11 `enum class MY_ENUM : unsigned __int64 { EN_NAME=0x1000000000000000ULL }`
// MY_ENUM::EN_NAME
enum opts
{
	OPT_SPEED      = 1,
	OPT_SPEEDTEXT  = OPT_SPEED      <<1,
	OPT_COURSE     = OPT_SPEEDTEXT  <<1,
	OPT_COURSETEXT = OPT_COURSE     <<1,
	OPT_TIME       = OPT_COURSETEXT <<1,
	OPT_COORDS     = OPT_TIME       <<1,
	
	OPT_ARGSEND    = OPT_COORDS     <<1,
	                 // define options WITH arguments ABOVE this, others below
	                 // ARGSEND is used to iterate over bits to generate a unordered_map
	                 // which contains any arguments given to the option
	                 // int=>char* (bitshifting over int to match opts)
	
	OPT_HELP       = OPT_ARGSEND    <<1,
	OPT_ERROR      = OPT_HELP       <<1,
	OPT_OPTEND     = OPT_ERROR      <<1
	                 // MUST be the last VALUE, determines the end for iterations (used?)
};




class Option
{
	private:
	         char* short_options;
	struct option* long_options;
	 unsigned int  bits = 0x0;
	std::unordered_map<int,const char*> arguments;
	
	public:
	            Option();
	           ~Option();
	      bool  load( int argc, char* argv[] );
	       int  get();
	      bool  isSet( opts option );
	const char* getArg( opts option );
};




#endif
// EOF
