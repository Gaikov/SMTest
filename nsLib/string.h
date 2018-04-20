#ifndef _NSSTRING_H_
#define _NSSTRING_H_

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define	MAX_STRING_SIZE	256	

class nsString
{
protected:
	char	m_str[MAX_STRING_SIZE];
	
public:
	inline nsString()
	{
		m_str[0] = 0;
	}
	
	inline explicit nsString( const char *str )
	{
		*this = str;
	}

	inline operator const char* ()
	{
		return m_str;
	}
	
	inline char* GetForChange()
	{
		return m_str;
	}
	
	inline nsString& operator = ( const char* str )
	{
		strncpy( m_str, str, MAX_STRING_SIZE - 1 );
		m_str[MAX_STRING_SIZE - 1] = 0;
		return *this;
	}
	
	inline bool operator == ( const char* str )
	{
		const char*	s1 = m_str;
		const char*	s2 = str;
		while ( *s1 && *s2 )
		{
			if ( *s1 != *s2 ) return false;
			s1++;
			s2++;
		}
		
		return *s1 == *s2;
	}
	
	inline bool operator == ( const nsString& str )
	{
		return *this == str.m_str;
	}
	
	inline void __cdecl Format( const char *fmt, ... )
	{
		va_list	list;
		va_start( list, fmt );
		vsprintf( m_str, fmt, list );
		va_end( list );
	}
};

#endif
