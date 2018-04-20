#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include "elocal.h"

class nsFileSystem : public IFileSystem  
{
public:
	static bool Init();
	static void Release();

private:
	nsFileSystem();
	virtual ~nsFileSystem();

	IFile*	LoadFile( const char* fileName );
	void	FreeFile( IFile* file );
};

extern	IFileSystem	*fs;

#endif
