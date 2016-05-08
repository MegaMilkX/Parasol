#ifndef _MACRO_COMPILE_TIME_MESSAGE_H_
#define _MACRO_COMPILE_TIME_MESSAGE_H_

#define STRINGIZE_HELPER(x) #x
#define STRINGIZE(x) STRINGIZE_HELPER(x)
#define __MESSAGE(text) __pragma( message(__FILE__ "(" STRINGIZE(__LINE__) ")" text) ) 
#define WARNING_(text) __MESSAGE( " : Warning: (" __FUNCTION__ "): " #text )
#define ERROR_(text) __MESSAGE( " : Error: (" __FUNCTION__ "): " #text )
#define MESSAGE_(text) __MESSAGE( ": (" __FUNCTION__ "): " #text )
#define TODO_(text) __MESSAGE( " : Warning: TODO: (" __FUNCTION__ ") " #text )

#endif