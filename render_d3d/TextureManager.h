#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

#include "RLocal.h"
#include "Texture.h"

class nsTextureManager
{
friend class nsRenderExport;

private:
	nsHashMap<nsTexture>	m_texList;

	D3DFORMAT				m_texFormat;
	D3DFORMAT				m_texFormatRGB;

	IDirect3DDevice8		*m_dev;

private:
	nsTextureManager();
	virtual ~nsTextureManager();

	bool	Init( IDirect3DDevice8* dev, D3DFORMAT backBuffFmt );
	void	Release( bool userRes );

	ITexture*		TextureLoad( const char* fileName, bool mipMap, textureFormat_t fmt );
	ITexture*		TextureCreate( const byte* data, int width, int height, bool mipMap, textureFormat_t fmt );
	void			TextureRelease( ITexture* tex );
	void			TextureBind( ITexture* tex );
};

#endif
