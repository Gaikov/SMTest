#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "elocal.h"

#define MAX_LOG_MSG	1024

class nsApplication : public IMainApp  
{
public:
	nsApplication();
	virtual ~nsApplication();

	bool Init();
	void Release();
	void OnIdle();

public:
	virtual void __cdecl Log( int prnLevel, const char *fmt, ... );
	virtual void __cdecl FatalError( const char *fmt, ... );
	virtual void Exit();

private:
	FILE*	m_log;
	
	DWORD	m_prevTime;

private:
	bool LogOpen();
	void LogClose();


};

#endif
