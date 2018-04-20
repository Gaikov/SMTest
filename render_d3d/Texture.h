#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "RLocal.h"

class nsTexture : public ITexture  
{
friend class nsTextureManager;
friend class nsRenderExport;

public:
	nsTexture	*prev, *next;

public:
	nsTexture( const char* fileName, D3DFORMAT fmt, bool mipMap );
	virtual ~nsTexture();

	void Reload( IDirect3DDevice8* dev );
	void Release();

private:
	char				m_fileName[MAX_STRING_SIZE];
	IDirect3DTexture8	*m_devTex;
	bool				m_mipMap;
	uint				m_width, m_height;
	int					m_refCount;
	D3DFORMAT			m_format;

private:
	virtual void GetSize( int& width, int& height );
	virtual void GetPicSize( int& width, int& height );
};

#endif
