#ifndef _RENDEREXPORT_H_
#define _RENDEREXPORT_H_

#include "RLocal.h"
#include "VertexBufferManager.h"
#include "States.h"
#include "TextureManager.h"

class nsRenderExport : public IRenderExport  
{
public:
	static IRenderExport*	Create();

	virtual bool					Init( uint hMainWnd );
	virtual void					Release();
	virtual const renderConfig_t*	GetCurrentConfig();
	
	//textures
	virtual ITexture*		TextureLoad( const char* fileName, bool mipMap, textureFormat_t fmt );
	virtual ITexture*		TextureCreate( const byte* data, int width, int height, bool mipMap, textureFormat_t fmt );
	virtual void			TextureRelease( ITexture* tex );
	virtual void			TextureBind( ITexture* tex );
	
	//render states
	virtual	void			SetAlphaMode( alphaMode_t mode );
	virtual void			SetRenderMode( renderMode_t mode );
	virtual void			SetZBufferState( bool test, bool write );
	virtual void			SetWorldMatrix( nsMatrix& m );
	
	//scene
	virtual bool			SceneBegin( nsVec3& camPos, nsVec3& camAt, nsVec3& camUp );
	virtual void			SceneClear( uint flags );
	virtual void			SceneEnd();
	virtual void			GetInvertView( nsMatrix& m );
	virtual void			GetView( nsMatrix& m );
	virtual nsVec3			ProjectVector( nsVec3& v );
	
	//
	virtual IVertexBuffer*	VerticesCreate( uint vertsCount, uint indexCount );
	virtual	void			VerticesRelease( IVertexBuffer* vb );
	virtual	void			VerticesDraw( IVertexBuffer* vb );
	
	//lighting
	virtual void			Lighting( bool enable );
	virtual void			LightEnable( int source, nsVec3& dir, nsColor& c );
	virtual void			LightDisable( int source );
	virtual void			LightAmbient( nsColor& c );

private:
	HWND					m_hWnd;
	IDirect3DDevice8		*m_dev;
	D3DDEVTYPE				m_devType;
	D3DPRESENT_PARAMETERS	m_presentParams;
	renderConfig_t			m_currConfig;
	bool					m_devLost;

	bool					m_restart;

	D3DXMATRIX				m_matProject, m_matView, m_matInvView, m_matIdent;
	D3DXVECTOR3				m_camPos, m_camUp, m_camAt;
	D3DCOLORVALUE			m_currColor;

	bool					m_lightEnabled;
	D3DCOLOR				m_lightAmbient;

	nsVertexBufferManager	m_vbManager;
	nsStates				*m_states;
	
	nsTextureManager		m_texManager;
	nsTexture				*m_currTexture;

	renderMode_t			m_renderMode;

private:
	nsRenderExport();
	virtual ~nsRenderExport();

	static void				RegisterVariables();

	void	CheckCaps( UINT dev, D3DDEVTYPE devType );
	bool	InitDevice();
	void	ReleaseDevice( bool delUserRes );
	void	AdjustWindow( HWND hWnd, int width, int height, bool fullscreen );
	bool	ResetDevice();

	void	InvalidateResources();
	bool	ReloadResources();
	//bool	ReloadResources();
	//void	SetBrightness( float value );
};

#endif
