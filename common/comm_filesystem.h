#ifndef _COMM_FS_H_
#define _COMM_FS_H_

//-----------------------------------------------------
//  IFile:  
//-----------------------------------------------------
struct IFile
{
	virtual const char* GetFileName() = 0;
	virtual const byte*	GetData() = 0;
	virtual uint		GetSize() = 0;
};

//-----------------------------------------------------
//  IFileSystem:  
//-----------------------------------------------------
struct IFileSystem
{
	virtual IFile*	LoadFile( const char* fileName ) = 0;	//возвращает 0 если файл не был загружен
	virtual void	FreeFile( IFile* file ) = 0;
};

#endif