#ifndef _COMM_APP_H_
#define _COMM_APP_H_

#define PRN_ALL		1
#define PRN_DEV		2
	
struct IMainApp
{
	virtual void __cdecl Log( int prnLevel, const char *fmt, ... ) = 0;	//prnLevel - комбинация флагов PRN_
	virtual void __cdecl FatalError( const char *fmt, ... ) = 0;
	virtual void Exit() = 0;
};

struct IInputSystem
{
};

#endif