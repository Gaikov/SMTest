#include "TextureManager.h"
#include "UtilsD3D.h"

nsTextureManager::nsTextureManager()
{

}

nsTextureManager::~nsTextureManager()
{

}

//-----------------------------------------------------
//  nsTextureManager::Init:  
//-----------------------------------------------------
bool nsTextureManager::Init( IDirect3DDevice8* dev, D3DFORMAT backBuffFmt )
{
	m_dev = dev;

	//texture format
	const formatDesc_t	*fmt;
	m_texFormat = utd3d->GetTextureFormat( (int)r_tex_bits->value, backBuffFmt );
	if ( !m_texFormat && r_tex_bits->value == 32 )
	{
		app->Log( PRN_ALL, "WARNING: 32 bit texture format unsupported, trying 16 bit!\n" );
		r_tex_bits->SetValue( 16 );
		m_texFormat = utd3d->GetTextureFormat( (int)r_tex_bits->value, backBuffFmt );
	}
	
	m_texFormatRGB = utd3d->GetTextureFormatRGB( (int)r_tex_bits->value, backBuffFmt );
	
	if ( fmt = utd3d->GetFormatDesc( m_texFormat ) )
		app->Log( PRN_ALL, "...using texture format for RGBA: %s (b:%i, c:%i)\n", fmt->d3dDesc, fmt->bits, fmt->components );
	if ( fmt = utd3d->GetFormatDesc( m_texFormatRGB ) )
		app->Log( PRN_ALL, "...using texture format for RGB: %s (b:%i, c:%i)\n", fmt->d3dDesc, fmt->bits, fmt->components );

	
	app->Log( PRN_ALL, "...reload textures\n" );
	nsTexture	*tex;
	m_texList.FetchBegin();
	while ( tex = m_texList.FetchNext() )
		tex->Reload( m_dev );
	m_texList.FetchEnd();
	
	return true;
}

//-----------------------------------------------------
//  nsTextureManager::Release:  
//-----------------------------------------------------
void nsTextureManager::Release( bool userRes )
{
	nsTexture	*tex;
	if ( userRes )
	{
		m_texList.FetchBegin();
		while ( tex = m_texList.FetchNext() )
		{
			tex->Release();
			delete tex;
		}
		m_texList.FetchEnd();
		m_texList.DeleteAllKeys();
	}
	else
	{
		m_texList.FetchBegin();
		while ( tex = m_texList.FetchNext() )
			tex->Release();
		m_texList.FetchEnd();
	}
}


//-----------------------------------------------------
//  nsTextureManager::TextureLoad:  
//-----------------------------------------------------
ITexture* nsTextureManager::TextureLoad( const char* fileName, bool mipMap, textureFormat_t fmt )
{
	nsTexture	*tex = m_texList.GetData( fileName );
	if ( tex )
	{
		tex->m_refCount ++;
		return tex;
	}

	tex = new nsTexture( fileName, fmt == TF_RGB ? m_texFormatRGB : m_texFormat, mipMap );
	tex->Reload( m_dev );
	m_texList.SetData( fileName, tex );
	return tex;
}

//-----------------------------------------------------
//  nsTextureManager::TextureCreate:  
//-----------------------------------------------------
ITexture* nsTextureManager::TextureCreate( const byte* data, int width, int height, bool mipMap, textureFormat_t fmt )
{
	return 0;
}

//-----------------------------------------------------
//  nsTextureManager::TextureRelease:  
//-----------------------------------------------------
void nsTextureManager::TextureRelease( ITexture* tex )
{
	nsTexture	*t = (nsTexture*)tex;
	t->m_refCount --;
	if ( t->m_refCount <= 0 )
	{
		m_texList.DeleteKey( t->m_fileName );
		delete t;
	}
}
