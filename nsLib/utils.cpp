#include "utils.h"
#include <stdio.h>
#include <stdarg.h>

//-----------------------------------------------------
//  StrPrintf:  
//-----------------------------------------------------
const char* __cdecl StrPrintf( const char* fmt, ... )
{
	static char str[1024];

	va_list	list;
	va_start( list, fmt );
	vsprintf( str, fmt, list );
	va_end( list );

	return str;
}

//-----------------------------------------------------
//  StrEqual:  
//-----------------------------------------------------
bool StrEqual( const char* s1, const char *s2 )
{
	while ( *s1 && *s2 )
	{
		if ( *s1 != *s2 ) return false;
		
		s1 ++;
		s2 ++;
	}
	return *s1 == *s2;	
}

//-----------------------------------------------------
//  StrHash:  
//-----------------------------------------------------
byte StrHash( const char* str )
{
	uint	hash = 0;
	while ( *str )
	{
		hash += *str;
		str++;
	}
	return byte(hash % 255);
}
