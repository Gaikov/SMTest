#include "RenderExport.h"
#include "UtilsD3D.h"

IGameVar	*r_fullscreen = 0;
IGameVar	*r_pixel_bits = 0;
IGameVar	*r_tex_bits = 0;
IGameVar	*r_tex_filter = 0;
IGameVar	*r_vsync = 0;
IGameVar	*r_brightness = 0;
IGameVar	*r_d3d_device = 0;
IGameVar	*r_width = 0;
IGameVar	*r_height = 0;
IGameVar	*r_refresh = 0;

IGameVar	*r_fov = 0;
IGameVar	*r_wireframe = 0;
IGameVar	*r_soft_texgen = 0;

nsRenderExport::nsRenderExport()
{
	m_hWnd = 0;
	m_dev = 0;
	m_devType = D3DDEVTYPE_HAL;
	memset( &m_presentParams, 0, sizeof(D3DPRESENT_PARAMETERS) );
	memset( &m_currConfig, 0, sizeof(renderConfig_t) );
	m_devLost = true;
	m_restart = false;
	m_states = 0;

	m_renderMode = RM_NATIVE;
	m_currTexture = 0;
}

nsRenderExport::~nsRenderExport()
{

}

//-----------------------------------------------------
//  nsRenderExport::Create:  
//-----------------------------------------------------
IRenderExport* nsRenderExport::Create()
{
	RegisterVariables();
	if ( !nsUtilsD3D::Create() ) return 0;
	return new nsRenderExport;
}

//-----------------------------------------------------
//  nsRenderExport::RegisterVariables():  
//-----------------------------------------------------
void nsRenderExport::RegisterVariables()
{
	r_fullscreen	= cfg->RegisterVariable( "r_fullscreen", "0", GVF_SAVABLE );
	r_pixel_bits	= cfg->RegisterVariable( "r_pixel_bits", "16", GVF_SAVABLE );
	r_tex_bits		= cfg->RegisterVariable( "r_tex_bits", "16", GVF_SAVABLE );
	r_tex_filter	= cfg->RegisterVariable( "r_tex_filter", "1", GVF_SAVABLE );
	r_vsync			= cfg->RegisterVariable( "r_vsync", "0", GVF_SAVABLE );
	r_brightness	= cfg->RegisterVariable( "r_brightness", "0.5", GVF_SAVABLE );
	r_d3d_device	= cfg->RegisterVariable( "r_d3d_device", "0", GVF_SAVABLE | GVF_READONLY );
	r_width			= cfg->RegisterVariable( "r_width", "800", GVF_SAVABLE );
	r_height		= cfg->RegisterVariable( "r_height", "600", GVF_SAVABLE );
	r_refresh		= cfg->RegisterVariable( "r_refresh", "0", GVF_SAVABLE | GVF_READONLY );
}

//-----------------------------------------------------
//  nsRenderExport::Init:  
//-----------------------------------------------------
bool nsRenderExport::Init( uint hMainWnd )
{
	m_hWnd = (HWND)hMainWnd;
	if ( !m_hWnd || !IsWindow( m_hWnd ) )
	{
		app->FatalError( "D3D need a window handle identifier\n" );
		return false;
	}

	app->Log( PRN_ALL,
		"------------------------------------\n"\
		" initialize Direct3D Render         \n"\
		"------------------------------------\n" );
	
	RegisterVariables();

	r_fov = cfg->RegisterVariable( "r_fov", "60" );
	r_wireframe = cfg->RegisterVariable( "r_wireframe", "0" );
	r_soft_texgen = cfg->RegisterVariable( "r_soft_texgen", "0" );

	return InitDevice();
}

//-----------------------------------------------------
//  nsRenderExport::Release:  
//-----------------------------------------------------
void nsRenderExport::Release()
{
	app->Log( PRN_ALL,
		"------------------------------------\n"\
		" uninitialize Direct3D Render       \n"\
		"------------------------------------\n" );

	ReleaseDevice( true );

	delete this;
	nsUtilsD3D::Release();
}

//-----------------------------------------------------
//  nsRenderExport::GetCurrentConfig:  
//-----------------------------------------------------
const renderConfig_t* nsRenderExport::GetCurrentConfig()
{
	return 0;
}

//-----------------------------------------------------
//  nsRenderExport::TextureLoad:  
//-----------------------------------------------------
ITexture* nsRenderExport::TextureLoad( const char* fileName, bool mipMap, textureFormat_t fmt )
{
	return m_texManager.TextureLoad( fileName, mipMap, fmt );
}

//-----------------------------------------------------
//  nsRenderExport::TextureCreate:  
//-----------------------------------------------------
ITexture* nsRenderExport::TextureCreate( const byte* data, int width, int height, bool mipMap, textureFormat_t fmt )
{
	return m_texManager.TextureCreate( data, width, height, mipMap, fmt );
}

//-----------------------------------------------------
//  nsRenderExport::TextureRelease:  
//-----------------------------------------------------
void nsRenderExport::TextureRelease( ITexture* tex )
{
	m_texManager.TextureRelease( tex );
}

//-----------------------------------------------------
//  nsRenderExport::TextureBind:  
//-----------------------------------------------------
void nsRenderExport::TextureBind( ITexture* tex )
{
	if ( !tex )
	{
		m_dev->SetTexture( 0, 0 );
		m_currTexture = 0;
	}
	else if ( tex != m_currTexture )
	{
		m_currTexture = (nsTexture*)tex;
		m_dev->SetTexture( 0, m_currTexture->m_devTex );
	}
}

//-----------------------------------------------------
//  nsRenderExport::SetAlphaMode:  
//-----------------------------------------------------
void nsRenderExport::SetAlphaMode( alphaMode_t mode )
{
	switch ( mode )
	{
	case ALPHA_OFF:
		m_dev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
		break;
	case ALPHA_GLASS:
		m_dev->ApplyStateBlock( m_states->m_sbAlphaGlass );
		break;
	case ALPHA_ADD:
		m_dev->ApplyStateBlock( m_states->m_sbAlphaAdd );
		break;
	case ALPHA_MUL:
		m_dev->ApplyStateBlock( m_states->m_sbAlphaMul );
		break;
	case ALPHA_LIGHTMAP:
		m_dev->ApplyStateBlock( m_states->m_sbAlphaLightMap );
		break;
	case ALPHA_SUBTRACT:
		m_dev->ApplyStateBlock( m_states->m_sbAlphaSubtract );
		break;
	}
}

//-----------------------------------------------------
//  nsRenderExport::SetRenderMode:  
//-----------------------------------------------------
void nsRenderExport::SetRenderMode( renderMode_t mode )
{
	switch ( mode )
	{
	case RM_ORTHO_640:
		m_dev->ApplyStateBlock( m_states->m_sbOrtho640 );
		break;
	case RM_ORTHO_REAL:
		m_dev->ApplyStateBlock( m_states->m_sbOrthoReal );
		break;
	case RM_FRUSTUM:
		m_dev->ApplyStateBlock( m_states->m_sbFrustrum );
		m_dev->SetTransform( D3DTS_PROJECTION, &m_matProject );
		m_dev->SetTransform( D3DTS_VIEW, &m_matView );
		break;
	case RM_NATIVE:
		m_dev->SetTransform( D3DTS_PROJECTION, &m_matIdent );
		m_dev->SetTransform( D3DTS_VIEW, &m_matIdent );
		break;
	default:
		return;
	}
	
	m_renderMode = mode;
}

//-----------------------------------------------------
//  nsRenderExport::SetZBufferState:  
//-----------------------------------------------------
void nsRenderExport::SetZBufferState( bool test, bool write )
{
	m_dev->SetRenderState( D3DRS_ZENABLE, test ? 1 : 0 );
	m_dev->SetRenderState( D3DRS_ZWRITEENABLE, write ? 1 : 0 );
}

//-----------------------------------------------------
//  nsRenderExport::SetWorldMatrix:  
//-----------------------------------------------------
void nsRenderExport::SetWorldMatrix( nsMatrix& m )
{
	m_dev->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&m );
}

//-----------------------------------------------------
//  nsRenderExport::SceneBegin:  
//-----------------------------------------------------
bool nsRenderExport::SceneBegin( nsVec3& camPos, nsVec3& camAt, nsVec3& camUp )
{
	if ( !m_dev ) return false;
	
	if ( m_restart )
	{
		m_restart = false;
		InvalidateRect( m_hWnd, 0, TRUE );
		ReleaseDevice( true );
		if ( !InitDevice() ) return false;
	}
	
	HRESULT	hr;
	
	hr = m_dev->TestCooperativeLevel();
	if ( hr == D3DERR_DEVICELOST  )
	{
		m_devLost = true;
		return false;
	}
	else if ( hr == D3DERR_DEVICENOTRESET )
	{
		m_devLost = true;
		if ( !ResetDevice() ) return false;
	}
	
	hr = m_dev->BeginScene();
	if ( FAILED(hr) )
	{
		utd3d->FatalError( "BeginScene", hr );
		return false;
	}
	
	D3DXMatrixPerspectiveFovRH( &m_matProject, float(M_PI * r_fov->value / 180.0f), 1.333333333333333333f, 0.1f, 10.0f );
	
	v3todx( m_camPos, camPos );
	v3todx( m_camAt, camAt );
	v3todx( m_camUp, camUp );
	
	D3DXMatrixLookAtRH( &m_matView, &m_camPos, &m_camAt, &m_camUp );
	
	float	det = 0;
	D3DXMatrixInverse( &m_matInvView, &det, &m_matView );
	
	if ( r_wireframe->value )
		m_dev->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	else
		m_dev->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	
	return true;
}

//-----------------------------------------------------
//  nsRenderExport::SceneClear:  
//-----------------------------------------------------
void nsRenderExport::SceneClear( uint flags )
{
	DWORD	f = 0;
	
	if ( flags & CLEAR_ZBUFF )
		f |= D3DCLEAR_ZBUFFER;
	
	if ( flags & CLEAR_CBUFF )
		f |= D3DCLEAR_TARGET;
	
	HRESULT	hr;
	hr = m_dev->Clear( 0, NULL,  f, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
	if ( FAILED(hr) ) utd3d->FatalError( "Clear", hr );
}

//-----------------------------------------------------
//  nsRenderExport::SceneEnd:  
//-----------------------------------------------------
void nsRenderExport::SceneEnd()
{
	HRESULT	hr;
	if ( !m_devLost )
	{
		hr = m_dev->EndScene();
		if ( FAILED(hr) )
		{
			utd3d->FatalError( "EndScene", hr );
			return;
		}
		
		hr = m_dev->Present( 0, 0, 0, 0 );
		if ( hr != D3DERR_DEVICELOST )
		{
			if ( FAILED(hr) )
			{
				utd3d->FatalError( "Present", hr );
				return;
			}
		}
		else
			return;
		
		/*if ( rs.m_prev_bright != r_brightness->value )
		{
			rs.SetBrightness( r_brightness->value );
			ri.printf( PRN_ALL, "bright\n" );
		}//*/
	}
}

//-----------------------------------------------------
//  nsRenderExport::GetInvertView:  
//-----------------------------------------------------
void nsRenderExport::GetInvertView( nsMatrix& m )
{
	m = (float*)m_matInvView;
}

//-----------------------------------------------------
//  nsRenderExport::GetView:  
//-----------------------------------------------------
void nsRenderExport::GetView( nsMatrix& m )
{
	m = (float*)m_matView;
}

nsVec3 nsRenderExport::ProjectVector( nsVec3& v )
{
	return nsVec3( 0, 0, 0 );
}

//-----------------------------------------------------
//  nsRenderExport::VerticesCreate:  
//-----------------------------------------------------
IVertexBuffer* nsRenderExport::VerticesCreate( uint vertsCount, uint indexCount )
{
	return m_vbManager.CreateVertexBuffer( vertsCount, indexCount );
}

//-----------------------------------------------------
//  nsRenderExport::VerticesRelease:  
//-----------------------------------------------------
void nsRenderExport::VerticesRelease( IVertexBuffer* vb )
{
	if ( !vb ) return;
	m_vbManager.ReleaseVertexBuffer( (nsVertexBuffer*)vb );
}

//-----------------------------------------------------
//  nsRenderExport::VerticesDraw:  
//-----------------------------------------------------
void nsRenderExport::VerticesDraw( IVertexBuffer* vb )
{
	if ( !vb ) return;
	nsVertexBuffer	*buf = (nsVertexBuffer*)vb;
	buf->Draw( m_dev );
}

//-----------------------------------------------------
//  nsRenderExport::Lighting:  
//-----------------------------------------------------
void nsRenderExport::Lighting( bool enable )
{
	m_dev->SetRenderState( D3DRS_LIGHTING, enable ? TRUE : FALSE );
}

//-----------------------------------------------------
//  nsRenderExport::LightEnable:  
//-----------------------------------------------------
void nsRenderExport::LightEnable( int source, nsVec3& dir, nsColor& c )
{
	if ( !m_lightEnabled ) return;
	
	D3DLIGHT8	lit;
	memset( &lit, 0, sizeof(lit) );
	lit.Type = D3DLIGHT_DIRECTIONAL;
	
	lit.Diffuse.r = c[0];
	lit.Diffuse.g = c[1];
	lit.Diffuse.b = c[2];
	lit.Diffuse.a = c[3];
	
	dir.Norm();
	lit.Direction.x = dir.x;
	lit.Direction.y = dir.y;
	lit.Direction.z = dir.z;
	
	m_dev->LightEnable( source, TRUE );
	m_dev->SetLight( source, &lit );
}

//-----------------------------------------------------
//  nsRenderExport::LightDisable:  
//-----------------------------------------------------
void nsRenderExport::LightDisable( int source )
{
	m_dev->LightEnable( source, FALSE );
}

//-----------------------------------------------------
//  nsRenderExport::LightAmbient:  
//-----------------------------------------------------
void nsRenderExport::LightAmbient( nsColor& c )
{
	m_lightAmbient = c.ToARGB();
}

//-----------------------------------------------------
//  nsRenderExport::CheckCaps( UINT dev, D3DDEVTYPE devType ):  
//-----------------------------------------------------
void nsRenderExport::CheckCaps( UINT dev, D3DDEVTYPE devType )
{
	app->Log( PRN_ALL, "...checking device caps\n" );
	D3DCAPS8	caps;
	memset( &caps, 0, sizeof(caps) );
	if ( !FAILED(utd3d->GetD3D()->GetDeviceCaps( dev, devType, &caps )) )
	{
		app->Log( PRN_ALL, "max texture size: %ix%i\n", caps.MaxTextureWidth, caps.MaxTextureHeight );
		
		m_currConfig.capsTexGen = caps.VertexProcessingCaps & D3DVTXPCAPS_TEXGEN ? true : false;
		app->Log( PRN_ALL, "vertex texgen: %s\n", m_currConfig.capsTexGen ? "supported" : "unsupported" );
		if ( r_soft_texgen->value && m_currConfig.capsTexGen )
		{
			app->Log( PRN_ALL, "...using software texgen\n" );
			m_currConfig.capsTexGen = false;
		}
	}
}

//-----------------------------------------------------
//  nsRenderExport::InitDevice:  
//-----------------------------------------------------
bool nsRenderExport::InitDevice()
{
	//initialize device
	HRESULT	hr;
	
	app->Log( PRN_ALL, "initialize device\n" );

	if ( r_d3d_device->value < 0 || r_d3d_device->value >= utd3d->GetD3D()->GetAdapterCount() )
		r_d3d_device->SetValue( 0 );

	D3DADAPTER_IDENTIFIER8	devid;
	memset( &devid, 0, sizeof(devid) );
	if ( !FAILED(utd3d->GetD3D()->GetAdapterIdentifier( (UINT)r_d3d_device->value, 0, &devid )) )
		app->Log( PRN_ALL, "...using device: %s\n", devid.Description );

	CheckCaps( (UINT)r_d3d_device->value, m_devType );

	if ( !utd3d->SetCurrentDevice( (UINT)r_d3d_device->value, m_devType ) ) return false;
	int idx = utd3d->FindUserMode( (uint)r_width->value, (uint)r_height->value );
	if ( idx < 0 ) utd3d->FindUserMode( 800, 600 );
	const vidModeDesc_t* mode = utd3d->GetModeDesc( idx );
	if ( !mode )
	{
		app->FatalError( "WARNING: video mode not found!\n" );
		return false;
	}

	m_currConfig.currVidMode = idx;
	r_width->SetValue( (float)mode->width );
	r_height->SetValue( (float)mode->height );
	r_refresh->SetValue( (float)mode->refresh );

	D3DDISPLAYMODE d3ddm;
	memset( &d3ddm, 0, sizeof(d3ddm) );
	hr = utd3d->GetD3D()->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );
	if ( FAILED(hr) )
	{
		utd3d->FatalError( "GetAdapterDisplayMode", hr );
		return false;
	}

	memset( &m_presentParams, 0, sizeof(m_presentParams) );
	m_presentParams.hDeviceWindow = m_hWnd;
	m_presentParams.EnableAutoDepthStencil = TRUE;
	m_presentParams.AutoDepthStencilFormat = D3DFMT_D16;

reset:
	AdjustWindow( m_hWnd, (int)r_width->value, (int)r_height->value, r_fullscreen->value != 0 );

	if ( r_fullscreen->value )
	{
		app->Log( PRN_ALL, "...init fullscreen mode: %ix%ix%i\n", (int)r_width->value, (int)r_height->value, (int)r_pixel_bits->value );
		D3DFORMAT	fmt = utd3d->FindFSBackBufferFormat( (int)r_pixel_bits->value );
		if ( !fmt && r_pixel_bits->value == 32 )
		{
			app->Log( PRN_ALL, "WARNING: 32 bit color depth unsupported, trying 16 bit!\n" );
			r_pixel_bits->SetValue( 16 );
			fmt = utd3d->FindFSBackBufferFormat( (int)r_pixel_bits->value );
		}

		const formatDesc_t	*f = utd3d->GetFormatDesc( fmt );
		if ( f )
			app->Log( PRN_ALL, "...using backbuffer format: %s (b:%i,c:%i)\n", f->d3dDesc, f->bits, f->components );

		m_presentParams.BackBufferWidth = (UINT)r_width->value;
		m_presentParams.BackBufferHeight = (UINT)r_height->value;
		m_presentParams.BackBufferFormat = fmt;
		m_presentParams.FullScreen_RefreshRateInHz = (UINT)r_refresh->value;
		m_presentParams.FullScreen_PresentationInterval = r_vsync->value ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
		m_presentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;

		m_currConfig.fullScreen = true;
	}
	else
	{
		app->Log( PRN_ALL, "...init windowed mode: %ix%i\n", (int)r_width->value, (int)r_height->value );

		if ( !utd3d->CheckWinBackBufferFormat( d3ddm.Format ) )
		{
			app->Log( PRN_ALL, "WARNING: desktop mode unsupport hardware acceleration!\n" );
			r_fullscreen->SetValue( 1 );
			goto reset;
		}
		
		const formatDesc_t	*f = utd3d->GetFormatDesc( d3ddm.Format );
		if ( f )
		{
			app->Log( PRN_ALL, "...using backbuffer format: %s (b:%i,c:%i)\n", f->d3dDesc, f->bits, f->components );
			r_pixel_bits->SetValue( (float)f->bits );
		}

		m_presentParams.Windowed   = TRUE;
		m_presentParams.SwapEffect = r_vsync->value ? D3DSWAPEFFECT_COPY_VSYNC : D3DSWAPEFFECT_FLIP;
		m_presentParams.BackBufferCount = 1;
		m_presentParams.BackBufferFormat = d3ddm.Format; 

		m_currConfig.fullScreen = false;
	}

	app->Log( PRN_ALL, "...create device\n" );
	hr = utd3d->GetD3D()->CreateDevice( (UINT)r_d3d_device->value, m_devType, m_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_presentParams, &m_dev );
	if ( FAILED(hr) )
	{
		hr = utd3d->GetD3D()->CreateDevice( (UINT)r_d3d_device->value, m_devType, m_hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_presentParams, &m_dev );
		if ( FAILED(hr) )
		{
			utd3d->FatalError( "CreateDevice", hr );
			return false;
		}
		app->Log( PRN_ALL, "...using software vertex processing\n" );
	}
	else
		app->Log( PRN_ALL, "...using hardware vertex processing\n" );

	if ( !ReloadResources() ) return false;
	//SetBrightness( r_brightness->value );
	m_devLost = false;
	return true;
}

//-----------------------------------------------------
//  nsRenderExport::ReleaseDevice:  
//-----------------------------------------------------
void nsRenderExport::ReleaseDevice( bool delUserRes )
{
	m_texManager.Release( delUserRes );
	m_vbManager.Release( delUserRes );
	if ( m_states )
	{
		nsStates::OnDestroyDevice( m_states );
		m_states = 0;
	}
	
	if ( m_dev )
	{
		app->Log( PRN_ALL, "...release device\n" );
		m_dev->Release();
		m_dev = 0;
	}
}

//-----------------------------------------------------
//  nsRenderExport::AdjustWindow:  
//-----------------------------------------------------
void nsRenderExport::AdjustWindow( HWND hWnd, int width, int height, bool fullscreen )
{
	RECT	rect;
	rect.left = rect.top = 0;
	rect.right = width;
	rect.bottom = height;
	
	AdjustWindowRect( &rect, GetWindowLong( hWnd, GWL_STYLE ), FALSE );
	
	SetWindowPos( hWnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED );	//style apply
	SetWindowPos( hWnd, fullscreen ? HWND_TOPMOST : HWND_NOTOPMOST, 
		0, 0, rect.right - rect.left, rect.bottom - rect.top, 
		SWP_NOCOPYBITS | SWP_SHOWWINDOW );	//position and z order apply//*/
	
	GetClientRect( hWnd, &rect );
	app->Log( PRN_DEV, "...client size: %ix%i\n", rect.right, rect.bottom );
}

//-----------------------------------------------------
//  nsRenderExport::ResetDevice:  
//-----------------------------------------------------
bool nsRenderExport::ResetDevice()
{
	if ( !m_devLost ) return true;
	HRESULT	hr;
	
	D3DDISPLAYMODE d3ddm;
	memset( &d3ddm, 0, sizeof(d3ddm) );
	hr = utd3d->GetD3D()->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );
	if ( FAILED(hr) )
	{
		utd3d->FatalError( "GetAdapterDisplayMode", hr );
		return false;
	}
	//UpdateCurrConfig( d3ddm );
	
	InvalidateResources();
	
	app->Log( PRN_ALL, "=== restoring device ===\n" );
	if ( !m_currConfig.fullScreen )
	{
		app->Log( PRN_ALL, "...in windows mode\n" );
		m_presentParams.BackBufferFormat = d3ddm.Format;
	}
	
	hr = m_dev->Reset( &m_presentParams );
	if ( hr != D3D_OK )
	{
		app->Log( PRN_ALL, "failed to reset: %s\n", utd3d->GetErrorDesc( hr ) );
		return false;
	}
	else
	{
		//AdjustWindow( m_hWnd, (int)r_width->value, (int)r_height->value, r_fullscreen->value != 0 );
		if ( !ReloadResources() ) return false;
		app->Log( PRN_ALL, "=== device restored successfull ===\n" );
		m_devLost = false;
	}
	
	return true;
}

//-----------------------------------------------------
//  nsRenderExport::InvalidateResources:  
//-----------------------------------------------------
void nsRenderExport::InvalidateResources()
{
	m_texManager.Release( false );
	m_vbManager.Release( false );

	if ( m_states )
	{
		nsStates::OnDestroyDevice( m_states );
		m_states = 0;
	}
}

//-----------------------------------------------------
//  nsRenderExport::ReloadResources:  
//-----------------------------------------------------
bool nsRenderExport::ReloadResources()
{
	if ( !(m_states = nsStates::OnCreateDevice( m_dev, &m_currConfig )) ) return false;
	
	D3DXMatrixIdentity( &m_matIdent );
	
	/*D3DMATERIAL8 mtrl;
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL8) );
	m_currColor.r = mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	m_currColor.g = mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	m_currColor.b = mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	m_currColor.a = mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	m_dev->SetMaterial( &mtrl );//*/
	
	//SetLightAmbient( m_litAmb );
	m_dev->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );
	
	m_lightEnabled = false;
	
	m_dev->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_dev->LightEnable( 0, FALSE );
	m_dev->LightEnable( 1, FALSE );
	m_dev->LightEnable( 2, FALSE );
	m_dev->LightEnable( 3, FALSE );
	//m_dev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL );

	if ( !m_vbManager.Init( m_dev ) ) return false;
	if ( !m_texManager.Init( m_dev, m_presentParams.BackBufferFormat ) ) return false;

	return true;
}


