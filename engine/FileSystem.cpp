#include "FileSystem.h"

IFileSystem	*fs = 0;

//-----------------------------------------------------
//  class nsFile:  
//-----------------------------------------------------
class nsFile : public IFile
{
friend class nsFileSystem;

	nsFile()
	{
		m_fileName[0] = 0;
		m_data = 0;
		m_size = 0;
	}

	~nsFile()
	{
		if ( m_data ) free( m_data );
	}

private:
	char	m_fileName[MAX_STRING_SIZE];
	byte*	m_data;
	uint	m_size;

private:
	virtual const char* GetFileName() { return m_fileName; }
	virtual const byte*	GetData() { return m_data; }
	virtual uint		GetSize() { return m_size; }
};

//-----------------------------------------------------
//  nsFileSystem::Init:  
//-----------------------------------------------------
bool nsFileSystem::Init()
{
	if ( !fs )
	{
		fs = new nsFileSystem;
		if ( !fs ) return false;
	}
	return true;
}

//-----------------------------------------------------
//  nsFileSystem::Release:  
//-----------------------------------------------------
void nsFileSystem::Release()
{
	if ( fs )
	{
		nsFileSystem	*native = (nsFileSystem*)fs;
		delete native;
	}
}

//-----------------------------------------------------
//  nsFileSystem::nsFileSystem:  
//-----------------------------------------------------
nsFileSystem::nsFileSystem()
{

}

//-----------------------------------------------------
//  nsFileSystem::~nsFileSystem:  
//-----------------------------------------------------
nsFileSystem::~nsFileSystem()
{

}

//-----------------------------------------------------
//  nsFileSystem::LoadFile:  
//-----------------------------------------------------
IFile* nsFileSystem::LoadFile( const char* fileName )
{
	nsFile	*file = 0;
	FILE	*fp = 0;
	uint	size = 0;
	
	try
	{
		if ( !(fp = fopen( fileName, "rb" )) )
			throw StrPrintf( "WARNING: can't open file: '%s'\n", fileName );
	
		file = new nsFile;
	
		fseek( fp, 0, SEEK_END );
		size = ftell( fp );
		fseek( fp, 0, SEEK_SET );

		if ( !size ) throw "WARNING: file size if zero\n";
	
		strcpy( file->m_fileName, fileName );
		file->m_size = size;
	
		file->m_data = (byte*)malloc( size + 1 );	// + 1 for file parsed as string
		file->m_data[size] = 0;					//for file as string
	
		if ( fread( file->m_data, size, 1, fp ) != 1 )
			throw StrPrintf( "WARNING: read file data: '%s'\n", fileName );

		fclose( fp );
	}
	catch ( const char* error )
	{
		app->Log( PRN_ALL, error );
		if ( fp ) fclose( fp );
		if ( file ) delete file;
		return 0;
	}

	return file;
}

//-----------------------------------------------------
//  nsFileSystem::FreeFile:  
//-----------------------------------------------------
void nsFileSystem::FreeFile( IFile* file )
{
	nsFile*	f = (nsFile*)file;
	delete f;
}

