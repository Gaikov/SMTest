#include "UtilsD3D.h"

extern	nsUtilsD3D	*utd3d = 0;

const errorDesc_t nsUtilsD3D::m_errorDesc[] = 
{
	{ D3D_OK, "No error occurred. " },
	{ D3DERR_CONFLICTINGRENDERSTATE, "The currently set render states cannot be used together. " },
	{ D3DERR_CONFLICTINGTEXTUREFILTER, "The current texture filters cannot be used together. " },
	{ D3DERR_CONFLICTINGTEXTUREPALETTE, "The current textures cannot be used simultaneously. This generally occurs when a multitexture device requires that all palletized textures simultaneously enabled also share the same palette. " },
	{ D3DERR_DEVICELOST, "The device is lost and cannot be restored at the current time, so rendering is not possible. " },
	{ D3DERR_DEVICENOTRESET, "The device cannot be reset. " },
	{ D3DERR_DRIVERINTERNALERROR, "Internal driver error. " },
	{ D3DERR_INVALIDCALL, "The method call is invalid. For example, a method's parameter may have an invalid value. " },
	{ D3DERR_INVALIDDEVICE, "The requested device type is not valid. " },
	{ D3DERR_MOREDATA, "There is more data available than the specified buffer size can hold. " },
	{ D3DERR_NOTAVAILABLE, "This device does not support the queried technique. " },
	{ D3DERR_NOTFOUND, "The requested item was not found. " },
	{ D3DERR_OUTOFVIDEOMEMORY, "Direct3D does not have enough display memory to perform the operation. " },
	{ D3DERR_TOOMANYOPERATIONS, "The application is requesting more texture-filtering operations than the device supports. " },
	{ D3DERR_UNSUPPORTEDALPHAARG, "The device does not support a specified texture-blending argument for the alpha channel. " },
	{ D3DERR_UNSUPPORTEDALPHAOPERATION, "The device does not support a specified texture-blending operation for the alpha channel. " },
	{ D3DERR_UNSUPPORTEDCOLORARG, "The device does not support a specified texture-blending argument for color values. " },
	{ D3DERR_UNSUPPORTEDCOLOROPERATION, "The device does not support a specified texture-blending operation for color values. " },
	{ D3DERR_UNSUPPORTEDFACTORVALUE, "The device does not support the specified texture factor value. " },
	{ D3DERR_UNSUPPORTEDTEXTUREFILTER, "The device does not support the specified texture filter. " },
	{ D3DERR_WRONGTEXTUREFORMAT, "The pixel format of the texture surface is not valid. " },
	{ E_FAIL, "An undetermined error occurred inside the Direct3D subsystem. " },
	{ E_INVALIDARG, "An invalid parameter was passed to the returning function " },
	//	{ E_INVALIDCALL, "The method call is invalid. For example, a method's parameter may have an invalid value. " },
	{ E_OUTOFMEMORY, "Direct3D could not allocate sufficient memory to complete the call. " },
	{ S_OK, "No error occurred. " }
};

const int nsUtilsD3D::m_errorCount = sizeof(nsUtilsD3D::m_errorDesc) / sizeof(errorDesc_t);

const formatDesc_t nsUtilsD3D::m_fmtDesc[] = 
{
	{ D3DFMT_UNKNOWN, 0, 0, "Default", "Surface format is unknown. " },
	{ D3DFMT_R8G8B8, 32, 3, "R8G8B8", "24-bit RGB pixel format. " },//real 24
	{ D3DFMT_A8R8G8B8, 32, 4, "A8R8G8B8", "32-bit ARGB pixel format with alpha. " },
	{ D3DFMT_X8R8G8B8, 32, 3, "X8R8G8B8", "32-bit RGB pixel format where 8 bits are reserved for each color. " },
	
	{ D3DFMT_A4R4G4B4, 16, 4, "A4R4G4B4", "16-bit ARGB pixel format. " },
	{ D3DFMT_A1R5G5B5, 16, 4, "A1R5G5B5", "16-bit pixel format where 5 bits are reserved for each color and 1 bit is reserved for alpha (transparent texel). " },
	{ D3DFMT_A8R3G3B2, 16, 4, "A8R3G3B2", "16-bit ARGB texture format. " },
	
	{ D3DFMT_R5G6B5, 16, 3, "R5G6B5", "16-bit RGB pixel format. " },
	{ D3DFMT_X1R5G5B5, 16, 3, "X1R5G5B5", "16-bit pixel format where 5 bits are reserved for each color. " },
	{ D3DFMT_X4R4G4B4, 16, 3, "X4R4G4B4", "16-bit RGB pixel format where 4 bits are reserved for each color. " },
	
	{ D3DFMT_R3G3B2, 16, 3, "R3G3B2", "8-bit RGB texture format. " },//real 8
	
	{ D3DFMT_A8, 8, 1, "A8", "8-bit alpha only. " },
	{ D3DFMT_A8P8, 16, 2, "A8P8", "Surface is 8-bit color indexed with 8 bits of alpha. " },
	{ D3DFMT_P8, 8, 1, "P8", "Surface is 8-bit color indexed. " },
	{ D3DFMT_L8, 8, 1, "L8", "8-bit luminance only. " },
	{ D3DFMT_A8L8, 16, 2, "A8L8", "16-bit alpha luminance. " },
	{ D3DFMT_A4L4, 8, 2, "A4L4", "8-bit alpha luminance. " },
	{ D3DFMT_V8U8, 16, 2, "V8U8", "16-bit bump-map format. " },
	{ D3DFMT_L6V5U5, 16, 2, "L6V5U5", "16-bit bump-map format with luminance. " },
	{ D3DFMT_X8L8V8U8, 32, 0, "X8L8V8U8", "32-bit bump-map format with luminance where 8 bits are reserved for each element. " },
	{ D3DFMT_Q8W8V8U8, 32, 0, "Q8W8V8U8", "32-bit bump-map format. " },
	{ D3DFMT_V16U16, 32, 0, "V16U16", "32-bit bump-map format. " },
	{ D3DFMT_W11V11U10, 32, 0, "W11V11U10", "32-bit bump-map format. " },
	{ D3DFMT_UYVY, 0, 0, "UYVY", "UYVY format (PC98 compliance). " },
	{ D3DFMT_YUY2, 0, 0, "YUY2", "YUY2 format (PC98 compliance). " },
	{ D3DFMT_DXT1, 0, 0, "DXT1", "DXT1 compression texture format. " },
	{ D3DFMT_DXT2, 0, 0, "DXT2", "DXT2 compression texture format. " },
	{ D3DFMT_DXT3, 0, 0, "DXT3", "DXT3 compression texture format. " },
	{ D3DFMT_DXT4, 0, 0, "DXT4", "DXT4 compression texture format. " },
	{ D3DFMT_DXT5, 0, 0, "DXT5", "DXT5 compression texture format. " },
	{ D3DFMT_D16_LOCKABLE, 16, 0, "D16_LOCKABLE", "16-bit z-buffer bit depth. This is an application-lockable surface format. " },
	{ D3DFMT_D32, 32, 0, "D32", "32-bit z-buffer bit depth. " },
	{ D3DFMT_D15S1, 16, 0, "D15S1", "16-bit z-buffer bit depth where 15 bits are reserved for the depth channel and 1 bit is reserved for the stencil channel. " },
	{ D3DFMT_D24S8, 32, 0, "D24S8", "32-bit z-buffer bit depth where 24 bits are reserved for the depth channel and 8 bits are reserved for the stencil channel. " },
	{ D3DFMT_D16, 16, 0, "D16", "16-bit z-buffer bit depth. " },
	{ D3DFMT_D24X8, 32, 0, "D24X8", "32-bit z-buffer bit depth where 24 bits are reserved for the depth channel. " },
	{ D3DFMT_D24X4S4, 32, 0, "D24X4S4", "32-bit z-buffer bit depth where 24 bits are reserved for the depth channel and 4 bits are reserved for the stencil channel. " },
	{ D3DFMT_VERTEXDATA, 0, 0, "VERTEXDATA", "Describes a vertex buffer surface. " },
	{ D3DFMT_INDEX16, 16, 0, "INDEX16", "16-bit index buffer bit depth. " },
	{ D3DFMT_INDEX32, 32, 0, "INDEX32", "32-bit index buffer bit depth. " },
};

const int nsUtilsD3D::m_fmtCount = sizeof(nsUtilsD3D::m_fmtDesc) / sizeof(formatDesc_t);

//-----------------------------------------------------
//  nsUtilsD3D::D3DCreate:  
//-----------------------------------------------------
bool nsUtilsD3D::Create()
{
	if ( !utd3d )
	{
		utd3d = new nsUtilsD3D;
		if ( !utd3d )
		{
			app->Log( PRN_ALL, "out of memory\n" );
			return false;
		}

		if ( !utd3d->m_d3d )
		{
			delete utd3d;
			return false;
		}
	}
	return true;
}

//-----------------------------------------------------
//  nsUtilsD3D::D3DRelease:  
//-----------------------------------------------------
void nsUtilsD3D::Release()
{
	if ( utd3d )
	{
		delete utd3d;
		utd3d = 0;
	}
}

//-----------------------------------------------------
//  nsUtilsD3D::nsUtilsD3D:  
//-----------------------------------------------------
nsUtilsD3D::nsUtilsD3D()
{
	app->Log( PRN_ALL, "...create d3d\n" );
	if ( !(m_d3d = Direct3DCreate8( D3D_SDK_VERSION )) )
		app->FatalError( "Can't create Direct3D8\n" );

	if ( r_d3d_device->value < 0 || r_d3d_device->value >= m_d3d->GetAdapterCount() )
		r_d3d_device->SetValue( 0 );

	m_modes = 0;
	m_modesCount = 0;

	if ( !SetCurrentDevice( (UINT)r_d3d_device->value, D3DDEVTYPE_HAL ) )
	{
		m_d3d->Release();
		m_d3d = 0;
	}
}

//-----------------------------------------------------
//  nsUtilsD3D::~nsUtilsD3D:  
//-----------------------------------------------------
nsUtilsD3D::~nsUtilsD3D()
{
	if ( m_modes )
	{
		free( m_modes );
		m_modes = 0;
		m_modesCount = 0;
	}

	if ( m_d3d )
	{
		app->Log( PRN_ALL, "...delete d3d\n" );
		m_d3d->Release();
		m_d3d = 0;
	}
}

//-----------------------------------------------------
//  bool nsUtilsD3D::SetCurrentDevice:  
//-----------------------------------------------------
bool nsUtilsD3D::SetCurrentDevice( UINT dev, D3DDEVTYPE devType )
{
	m_devID = dev;
	m_devType = devType;
	return EnumModes();
}

//-----------------------------------------------------
//  nsUtilsD3D::GetErrorDesc:  
//-----------------------------------------------------
const char* nsUtilsD3D::GetErrorDesc( HRESULT hr ) const
{
	for ( int i = 0; i < m_errorCount; i++ )
		if ( m_errorDesc[i].code == hr )
			return m_errorDesc[i].desc;
	return "Unknown error";
}

//-----------------------------------------------------
//  nsUtilsD3D::GetFormatDesc:  
//-----------------------------------------------------
const formatDesc_t* nsUtilsD3D::GetFormatDesc( D3DFORMAT fmt ) const
{
	for ( int i = 0; i < m_fmtCount; i++ )
		if ( m_fmtDesc[i].fmt == fmt )
			return &m_fmtDesc[i];
	return 0;
}

//-----------------------------------------------------
//  nsUtilsD3D::FatalError:  
//-----------------------------------------------------
void nsUtilsD3D::FatalError( const char* funcName, HRESULT hr )
{
	app->FatalError( "D3D FATAL ERROR: %s: %s", funcName, GetErrorDesc( hr ) );
}

//-----------------------------------------------------
//  nsUtilsD3D::FindUserMode:  
//-----------------------------------------------------
int nsUtilsD3D::FindUserMode( uint width, uint height )
{
	if ( !m_modes ) return -1;

	for ( int i = 0; i < m_modesCount; i++ )
	{
		if ( m_modes[i].width == width &&
			m_modes[i].height == height )
			return i;
	}
	return -1;
}

//-----------------------------------------------------
//  nsUtilsD3D::GetModeDesc:  
//-----------------------------------------------------
const vidModeDesc_t* nsUtilsD3D::GetModeDesc( int index )
{
	if ( index < 0 || index >= m_modesCount || !m_modes ) return 0;
	return &m_modes[index];
}

//-----------------------------------------------------
//  nsUtilsD3D::FindBackBufferFormat:  
//-----------------------------------------------------
D3DFORMAT nsUtilsD3D::FindFSBackBufferFormat( int colorBits )
{
	HRESULT	hr;
	for ( int i = 0; i < m_fmtCount; i++ )
	{
		if ( m_fmtDesc[i].bits == colorBits )
		{
			hr = m_d3d->CheckDeviceType( m_devID, m_devType, m_fmtDesc[i].fmt, m_fmtDesc[i].fmt, FALSE );
			if ( SUCCEEDED(hr) ) return m_fmtDesc[i].fmt;
		}
	}

	return D3DFMT_UNKNOWN;
}

//-----------------------------------------------------
//  nsUtilsD3D::CheckWinBackBufferFormat:  
//-----------------------------------------------------
bool nsUtilsD3D::CheckWinBackBufferFormat( D3DFORMAT fmt )
{
	if ( SUCCEEDED(m_d3d->CheckDeviceType( m_devID, m_devType, fmt, fmt, TRUE )) ) return true;
	return false;
}

//-----------------------------------------------------
//  nsUtilsD3D::EnumModes:  
//-----------------------------------------------------
bool nsUtilsD3D::EnumModes()
{
	if ( m_modes )
	{
		free( m_modes );
		m_modes = 0;
		m_modesCount = 0;
	}

	D3DDISPLAYMODE	m;
	
	app->Log( PRN_ALL, "...enum modes\n" );
	UINT	idx = 0;
	while ( m_d3d->EnumAdapterModes( m_devID, idx, &m ) == D3D_OK )
	{
		vidModeDesc_t	mode;
		mode.width = m.Width;
		mode.height = m.Height;
		mode.refresh = m.RefreshRate;

		bool	add = true;
		for ( int i = 0; i < m_modesCount; i++ )
		{
			if ( m_modes[i].width == mode.width && m_modes[i].height == mode.height )
			{
				if ( m_modes[i].refresh < mode.refresh )
					m_modes[i] = mode;
				add = false;
				break;
			}
		}
		
		if ( add )
		{
			//app->Log( PRN_DEV, "%ix%i (%i Hz)\n", mode.width, mode.height, mode.refresh );
			AddToArray( &m_modes, m_modesCount, mode );
		}
		
		idx ++;
	}
	
	return m_modesCount != 0;
}

//-----------------------------------------------------
//  nsUtilsD3D::GetTextureFormat:  
//-----------------------------------------------------
D3DFORMAT nsUtilsD3D::GetTextureFormat( int texBits, D3DFORMAT backBufferFmt )
{
	for ( int i = 0; i < m_fmtCount; i++ )
	{
		if ( texBits == m_fmtDesc[i].bits )
			if ( m_d3d->CheckDeviceFormat( 0, m_devType, backBufferFmt, 0, D3DRTYPE_TEXTURE, m_fmtDesc[i].fmt ) == D3D_OK )
				return m_fmtDesc[i].fmt;
	}
	
	return D3DFMT_UNKNOWN;
}

//-----------------------------------------------------
//  nsUtilsD3D::GetTextureFormatRGB:  
//-----------------------------------------------------
D3DFORMAT nsUtilsD3D::GetTextureFormatRGB( int texBits, D3DFORMAT backBufferFmt )
{
	for ( int i = 0; i < m_fmtCount; i++ )
	{
		if ( texBits == m_fmtDesc[i].bits && m_fmtDesc[i].components == 3 )
			if ( m_d3d->CheckDeviceFormat( 0, m_devType, backBufferFmt, 0, D3DRTYPE_TEXTURE, m_fmtDesc[i].fmt ) == D3D_OK )
				return m_fmtDesc[i].fmt;
	}
	
	return GetTextureFormat( texBits, backBufferFmt );
}

