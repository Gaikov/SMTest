#include "States.h"
#include "UtilsD3D.h"

//-----------------------------------------------------
//  nsStates::OnCreateDevice:  
//-----------------------------------------------------
nsStates* nsStates::OnCreateDevice( IDirect3DDevice8* dev, const renderConfig_t *currCfg )
{
	if ( !dev ) return 0;
	
	nsStates	*sb;
	if ( !(sb = new nsStates) ) return 0;
	sb->m_dev = dev;
	sb->m_cfg = currCfg;
	
	if ( !sb->InitStates() )
	{
		delete sb, sb = 0;
		return 0;
	}
	
	return sb;
}

//-----------------------------------------------------
//  nsStates::Release:  
//-----------------------------------------------------
void nsStates::OnDestroyDevice( nsStates* sb )
{
	sb->FreeStates();
	delete sb;
}

//-----------------------------------------------------
//  nsStates::nsStates:  
//-----------------------------------------------------
nsStates::nsStates()
{
	m_dev = 0;
}

//-----------------------------------------------------
//  nsStates::~nsStates:  
//-----------------------------------------------------
nsStates::~nsStates()
{
	m_dev = 0;
}

//-----------------------------------------------------
//  nsStates::InitStates:  
//-----------------------------------------------------
bool nsStates::InitStates()
{
	app->Log( PRN_ALL, "...init device states\n" );
	m_dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );

	if ( !InitRenderMode() ) return false;
	if ( !InitAlphaModes() ) return false;
	if ( !InitTexModes() ) return false;
	if ( !InitTexFilter() ) return false;

	return true;
}

//-----------------------------------------------------
//  nsStates::FreeStates:  
//-----------------------------------------------------
void nsStates::FreeStates()
{
	app->Log( PRN_ALL, "...release device states\n" );
}

//-----------------------------------------------------
//  nsStates::InitTexFilter:  
//-----------------------------------------------------
bool nsStates::InitTexFilter()
{
	if ( !BeginState( m_dev ) ) return false;
	m_dev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	m_dev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	m_dev->SetTextureStageState( 0, D3DTSS_MIPFILTER, r_tex_filter->value ? D3DTEXF_LINEAR : D3DTEXF_POINT );//*/
	if ( !EndState( m_dev, m_sbTexLinear ) ) return false;
	
	if ( !BeginState( m_dev ) ) return false;
		m_dev->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
		m_dev->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
		m_dev->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_POINT );
	if ( !EndState( m_dev, m_sbTexNearest ) ) return false;

	m_dev->ApplyStateBlock( m_sbTexLinear );
	return true;
}

//-----------------------------------------------------
//  nsStates::InitRenderMode:  
//-----------------------------------------------------
bool nsStates::InitRenderMode()
{
	D3DXMATRIX	ident, ortho;
	D3DXMatrixOrthoOffCenterRH( &ortho, 0, 639, 479, 0, -1, 1 );
	D3DXMatrixIdentity( &ident );
	
	//Ortho640
	if ( !BeginState( m_dev ) ) return false;
	m_dev->SetTransform( D3DTS_VIEW, &ident );
	m_dev->SetTransform( D3DTS_PROJECTION, &ortho );
	m_dev->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	m_dev->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_FLAT );	//enable it on 3D
	m_dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );		//enable it on 3D (CW)
	m_dev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	if ( !EndState( m_dev, m_sbOrtho640 ) ) return false;
	
	//OrthoReal
	D3DXMatrixOrthoOffCenterRH( &ortho, 0, r_width->value, 0, r_height->value, -1, 1 );
	if ( !BeginState( m_dev ) ) return false;
	m_dev->SetTransform( D3DTS_VIEW, &ident );
	m_dev->SetTransform( D3DTS_PROJECTION, &ortho );
	m_dev->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	//m_dev->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_dev->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_FLAT );	//enable it on 3D
	m_dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );		//enable it on 3D (CW)
	if ( !EndState( m_dev, m_sbOrthoReal ) ) return false;
	
	//Frustrum
	if ( !BeginState( m_dev ) ) return false;
	m_dev->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	m_dev->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );	//enable it on 3D
	m_dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );		//enable it on 3D (CW)
	//m_dev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
	if ( !EndState( m_dev, m_sbFrustrum ) ) return false;

	return true;
}

//-----------------------------------------------------
//  nsStates::InitAlphaModes:  
//-----------------------------------------------------
bool nsStates::InitAlphaModes()
{
	//AlphaGlass
	if ( !BeginState( m_dev ) ) return false;
	m_dev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	m_dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	if ( !EndState( m_dev, m_sbAlphaGlass ) ) return false;
	
	//AlphaAdd
	if ( !BeginState( m_dev ) ) return false;
	m_dev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	m_dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
	if ( !EndState( m_dev, m_sbAlphaAdd ) ) return false;
	
	//AlphaMul
	if ( !BeginState( m_dev ) ) return false;
	m_dev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR );
	m_dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVDESTCOLOR );
	if ( !EndState( m_dev, m_sbAlphaMul ) ) return false;
	
	//AlphaLightMap
	if ( !BeginState( m_dev ) ) return false;
	m_dev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR );
	m_dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR );
	if ( !EndState( m_dev, m_sbAlphaLightMap ) ) return false;

	//AlphaSubtract
	if ( !BeginState( m_dev ) ) return false;
	m_dev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
	m_dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR );
	if ( !EndState( m_dev, m_sbAlphaSubtract ) ) return false;

	return true;
}

//-----------------------------------------------------
//  nsStates::InitTexModes:  
//-----------------------------------------------------
bool nsStates::InitTexModes()
{
	if ( !BeginState( m_dev ) ) return false;
	m_dev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	m_dev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_dev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	
	m_dev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	m_dev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_dev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	
	//disable proj
	m_dev->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP );
	m_dev->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP );
	m_dev->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU );
	
	//disable transform
	m_dev->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
	if ( !EndState( m_dev, m_sbTexNormal ) ) return false;
	
	if ( !BeginState( m_dev ) ) return false;
	m_dev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	m_dev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_dev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	m_dev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	if ( !EndState( m_dev, m_sbTexReplace ) ) return false;
	
	if ( !BeginState( m_dev ) ) return false;
	m_dev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	m_dev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_dev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	m_dev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	if ( !EndState( m_dev, m_sbTexModul ) ) return false;
	
	m_dev->ApplyStateBlock( m_sbTexNormal );
	
	if ( m_cfg->capsTexGen )
	{
		if ( !BeginState( m_dev ) ) return false;
		m_dev->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
		m_dev->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );//*/
		m_dev->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION );
		m_dev->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT4 );//*/
		if ( !EndState( m_dev, m_sbTexProj ) ) return false;
		
		if ( !BeginState( m_dev ) ) return false;
		D3DXMATRIX mat;
		mat._11 = 0.5f; mat._12 = 0.0f; mat._13 = 0.0f; mat._14 = 0.0f; 
		mat._21 = 0.0f; mat._22 =-0.5f; mat._23 = 0.0f; mat._24 = 0.0f; 
		mat._31 = 0.0f; mat._32 = 0.0f; mat._33 = 1.0f; mat._34 = 0.0f; 
		mat._41 = 0.5f; mat._42 = 0.5f; mat._43 = 0.0f; mat._44 = 1.0f; 
		m_dev->SetTransform( D3DTS_TEXTURE0, &mat );
		m_dev->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );
		m_dev->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL );
		if ( !EndState( m_dev, m_sbTexSphere ) ) return false;
	}
	else
	{
		m_sbTexProj = 0;
		m_sbTexSphere = 0;
	}
	
	if ( !BeginState( m_dev ) ) return false;
	m_dev->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
	m_dev->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );
	if ( !EndState( m_dev, m_sbTexClamp ) ) return false;
	
	if ( !BeginState( m_dev ) ) return false;
	m_dev->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP );
	m_dev->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP );
	if ( !EndState( m_dev, m_sbTexWrap ) ) return false;

	return true;
}

//-----------------------------------------------------
//  nsStates::BeginState:  
//-----------------------------------------------------
bool nsStates::BeginState( IDirect3DDevice8* dev )
{
	HRESULT	hr;
	hr = dev->BeginStateBlock(); 
	if ( FAILED(hr) )
	{
		utd3d->FatalError( "BeginStateBlock", hr );
		return false;
	}
	
	return true;
}

//-----------------------------------------------------
//  nsStates::EndState:  
//-----------------------------------------------------
bool nsStates::EndState( IDirect3DDevice8 *dev, DWORD &state)
{
	HRESULT	hr;
	hr = dev->EndStateBlock( &state ); 
	if ( FAILED(hr) )
	{
		utd3d->FatalError( "EndStateBlock", hr );
		return false;
	}
	
	return true;
}
