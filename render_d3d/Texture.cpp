#include "Texture.h"
#include "UtilsD3D.h"

//-----------------------------------------------------
//  nsTexture::nsTexture:  
//-----------------------------------------------------
nsTexture::nsTexture( const char* fileName, D3DFORMAT fmt, bool mipMap )
{
	strcpy( m_fileName, fileName );
	m_format = fmt;
	m_mipMap = mipMap;
	m_refCount = 1;
	m_width = m_height = 0;
	m_devTex = 0;
}

//-----------------------------------------------------
//  nsTexture::~nsTexture:  
//-----------------------------------------------------
nsTexture::~nsTexture()
{
	Release();
}

//-----------------------------------------------------
//  nsTexture::Reload:  
//-----------------------------------------------------
void nsTexture::Reload( IDirect3DDevice8* dev )
{
	Release();

	IFile	*file = 0;
	HRESULT	hr;
	bool	notex = false;
	
	if ( !(file = fs->LoadFile( m_fileName ) ) ) return;
	
	D3DPOOL	pool = D3DPOOL_MANAGED;
	
	hr = D3DXCreateTextureFromFileInMemoryEx( dev, file->GetData(), file->GetSize(),
		D3DX_DEFAULT, D3DX_DEFAULT, 
		m_mipMap ? 0 : 1, 0, m_format, pool,
		D3DX_DEFAULT, D3DX_DEFAULT,
		0, 0, 0, &m_devTex );
	
	if ( FAILED(hr) )
	{
		app->Log( PRN_ALL, "D3DXCreateTextureFromFileInMemoryEx (): %s\n", utd3d->GetErrorDesc( hr ) );
		m_devTex = 0;
	}
	else	//get texture size
	{
		D3DSURFACE_DESC		surf_desc;
		memset( &surf_desc, 0, sizeof(surf_desc) );
		hr = m_devTex->GetLevelDesc( 0, &surf_desc );
		if ( FAILED(hr) )
			app->Log( PRN_ALL, "WARNING: can't retrieve texture size\n" );
		else
		{
			m_width = surf_desc.Width;
			m_height = surf_desc.Height;
		}
	}
	
	fs->FreeFile( file );
}

//-----------------------------------------------------
//  nsTexture::Release:  
//-----------------------------------------------------
void nsTexture::Release()
{
	if ( m_devTex )
	{
		m_devTex->Release();
		m_devTex = 0;
	}
	m_width = 0;
	m_height = 0;
}

//-----------------------------------------------------
//  nsTexture::GetSize:  
//-----------------------------------------------------
void nsTexture::GetSize( int& width, int& height )
{
	width = m_width;
	height = m_height;
}

//-----------------------------------------------------
//  nsTexture::GetPicSize:  
//-----------------------------------------------------
void nsTexture::GetPicSize( int& width, int& height )
{
	width = m_width;
	height = m_height;
}

