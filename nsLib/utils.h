#ifndef _UTILS_H_
#define _UTILS_H_

#include "comm_types.h"

#define	M_PI 3.1415926535897932384626433832795f

const char* __cdecl StrPrintf( const char* fmt, ... );
bool		StrEqual( const char* s1, const char *s2 );
byte		StrHash( const char* str );

//-----------------------------------------------------
//  AddToArray:  
//-----------------------------------------------------
template<class T>
inline bool AddToArray( T** array, int& count, T& data )
{
	if ( !*array )
	{
		count = 0;
		*array = (T*)malloc( sizeof(T) );
	}
	else
		*array = (T*)realloc( *array, sizeof(T) * (count + 1) );
	
	if ( !*array ) return false;
	
	memcpy( (*array) + count, &data, sizeof(T) );
	count++;
	return true;
}

//-----------------------------------------------------
//  AddToList:  
//-----------------------------------------------------
template <class T>
inline void AddToList( T** lst, T* data )
{
	if ( *lst ) (*lst)->prev = data;
	data->next = *lst;
	(*lst) = data;
}


//-----------------------------------------------------
//  DelFromList:  
//-----------------------------------------------------
template <class T>
inline void DelFromList( T** lst, T* data )
{
	if ( *lst == data ) *lst = data->next;
	T* prev = data->prev;
	T* next = data->next;
	if ( prev ) prev->next = next;
	if ( next ) next->prev = prev;
}

inline float MathAbsf( float val )
{
	if ( val < 0 ) val = -val;
	return val;
}


#endif