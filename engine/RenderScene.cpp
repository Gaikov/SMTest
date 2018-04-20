#include "RenderScene.h"
#include "Wnd.h"
#include "Config.h"
#include "Camera.h"
#include "FileSystem.h"

const float	SCENE_CUBE_SIZE	= 2.0f;
const float	SCENE_HALF_SIZE = SCENE_CUBE_SIZE * 0.5f;

bool	g_drawSpriteParts = true;
bool	g_drawCubeParts = true;

#define	MAX_VB_SPRITE_PARTICLES	10000
#define	MAX_VB_CUBE_PARTICLES	1800

#ifdef _DEBUG
#define	RENDER_DLL	"render_d3d_d.dll"
#else
#define RENDER_DLL	"render_d3d.dll"
#endif

inline void MakeCubeSide( vbVertex_t *v, const nsVec3& x, const nsVec3& y, const nsVec3& z, const nsVec3& pos, float halfSize )
{
	v[0].v = ( -x - y + z ) * halfSize + pos;
	v[1].v = ( x - y + z ) * halfSize + pos;
	v[2].v = ( x + y + z ) * halfSize + pos;
	v[3].v = ( -x + y + z ) * halfSize + pos;
}

//////////////////////////////////////////////////////////////////////////

nsRenderScene	*rend = 0;

nsRenderScene::nsRenderScene()
{
	m_renderDll = 0;
	m_re = 0;
	m_coordSys = 0;
	m_aux = 0;
	m_wireCube = 0;
	m_spriteParts = 0;
	m_texSprite = 0;
	m_texCube = 0;
}

nsRenderScene::~nsRenderScene()
{

}

//-----------------------------------------------------
//  nsRenderScene::Init:  
//-----------------------------------------------------
bool nsRenderScene::Init()
{
	if ( !rend )
	{
		rend = new nsRenderScene;
		if ( !rend ) return false;
		if ( !rend->OnInit() )
			return false;
	}
	return true;
}

//-----------------------------------------------------
//  nsRenderScene::Release:  
//-----------------------------------------------------
void nsRenderScene::Release()
{
	if ( rend )
	{
		rend->OnRelease();
		delete rend;
		rend = 0;
	}
}

//-----------------------------------------------------
//  nsRenderScene::Init:  
//-----------------------------------------------------
bool nsRenderScene::OnInit()
{
	if ( !g_mainWnd.Create( APP_FULL_NAME, "SM_TEST_CLASSNAME" ) ) return false;
	if ( !LoadAPI( RENDER_DLL ) ) return false;
	if ( !m_re->Init( (uint)g_mainWnd.m_hWnd ) ) return false;

	if ( !(m_aux = new nsRenderAux( m_re )) )
	{
		app->FatalError( "Can't create render aux!\n" );
		return false;
	}

	if ( !(m_coordSys = m_aux->BuildCoordSystem()) ) return false;
	if ( !(m_wireCube = m_aux->BuildWireCube( SCENE_CUBE_SIZE )) ) return false;

	//build sprites vertices
	if ( !(m_spriteParts = m_re->VerticesCreate( MAX_VB_SPRITE_PARTICLES * 4, MAX_VB_SPRITE_PARTICLES * 6 )) ) return false;
	word		*idx = m_spriteParts->GetWriteIndices();
	vbVertex_t	*v = m_spriteParts->GetWriteVertices();
	for ( int i = 0; i < MAX_VB_SPRITE_PARTICLES; i++, idx += 6, v += 4 )
	{
		word	offs = i * 4;
		idx[0] = offs;
		idx[1] = offs + 1;
		idx[2] = offs + 2;

		idx[3] = offs;
		idx[4] = offs + 2;
		idx[5] = offs + 3;

		v[0].tu = 0;	v[0].tv = 1;
		v[1].tu = 1;	v[1].tv = 1;
		v[2].tu = 1;	v[2].tv = 0;
		v[3].tu = 0;	v[3].tv = 0;
	}

	//build cube vertices
	if ( !(m_cubeParts = m_re->VerticesCreate( MAX_VB_CUBE_PARTICLES * 24, MAX_VB_CUBE_PARTICLES * 36 )) ) return false;
	idx = m_cubeParts->GetWriteIndices();
	v = m_cubeParts->GetWriteVertices();

	for ( i = 0; i < MAX_VB_CUBE_PARTICLES; i++ )
	{
		for ( int j = 0; j < 6; j++, v += 4 )
		{
			v[0].tu = 0;	v[0].tv = 1;
			v[1].tu = 1;	v[1].tv = 1;
			v[2].tu = 1;	v[2].tv = 0;
			v[3].tu = 0;	v[3].tv = 0;
		}

		for ( j = 0; j < 6; j++, idx += 6 )
		{
			word	offs = i * 24 + j * 4;
			idx[0] = offs;
			idx[1] = offs + 1;
			idx[2] = offs + 2;
			
			idx[3] = offs;
			idx[4] = offs + 2;
			idx[5] = offs + 3;
		}
	}

	m_texSprite = m_re->TextureLoad( "textures/star.tga", false, TF_RGBA );
	m_texCube = m_re->TextureLoad( "textures/cube.jpg", true, TF_RGB );
	m_texTest = m_re->TextureLoad( "textures/console.jpg", true, TF_RGB );
	return true;
}

//-----------------------------------------------------
//  nsRenderScene::Loop:  
//-----------------------------------------------------
void nsRenderScene::Loop()
{
	m_psSprite.Loop();
	m_psCube.Loop();
}

//-----------------------------------------------------
//  nsRenderScene::Release:  
//-----------------------------------------------------
void nsRenderScene::OnRelease()
{
	if ( m_aux )
	{
		delete m_aux;
		m_aux = 0;
	}

	if ( m_renderDll )
	{
		m_re->Release();
		m_re = 0;

		app->Log( PRN_ALL, "...unloading render\n" );
		FreeLibrary( m_renderDll );
	}

	g_mainWnd.Destroy();
}

//-----------------------------------------------------
//  nsRenderScene::DrawWorld:  
//-----------------------------------------------------
void nsRenderScene::DrawWorld()
{
	nsMatrix	matView;
	
	if ( !m_re->SceneBegin( g_mainCam.m_pos, g_mainCam.m_at, g_mainCam.m_up ) ) return;
	
	m_re->GetView( matView );
	nsVec3		camUp( matView._12, matView._22, matView._32 );
	nsVec3		camRight( matView._11, matView._21, matView._31 );

	m_re->SetRenderMode( RM_FRUSTUM );
	m_re->SceneClear( CLEAR_ZBUFF | CLEAR_CBUFF );
	m_re->Lighting( false );
	m_re->TextureBind( 0 );
	m_re->SetZBufferState( true, true );
	m_re->SetAlphaMode( ALPHA_OFF );

	nsMatrix	m;
	m.Identity();
	m_re->SetWorldMatrix( m );
	m_re->VerticesDraw( m_wireCube );
	//m_re->VerticesDraw( m_coordSys );

	//draw cube particles
	//if ( 0 )
	{
		uint	cubeCount = m_psCube.GetParticlesCount();
		if ( cubeCount > MAX_VB_CUBE_PARTICLES ) cubeCount = MAX_VB_CUBE_PARTICLES;
		m_cubeParts->SetValidVertices( cubeCount * 24 );
		m_cubeParts->SetValidIndices( cubeCount * 36 );

		vbVertex_t			*v = m_cubeParts->GetWriteVertices();
		const particle_t	*p = m_psCube.GetParticles();

		nsVec3	x( 1, 0, 0 );
		nsVec3	y( 0, 1, 0 );
		nsVec3	z( 0, 0, 1 );

		for ( uint i = 0; i < cubeCount; i++, p++, v += 24 )
		{
			float	halfSize = p->size * 0.5f;
			MakeCubeSide( v, x, y, z, p->pos, halfSize );		//front
			MakeCubeSide( v + 4, -x, y, -z, p->pos, halfSize );	//back
			MakeCubeSide( v + 8, x, -z, y, p->pos, halfSize );	//top
			MakeCubeSide( v + 12, x, z, -y, p->pos, halfSize );	//bottom
			MakeCubeSide( v + 16, z, y, -x, p->pos, halfSize );	//left
			MakeCubeSide( v + 20, -z, y, x, p->pos, halfSize );	//right
		}

		m_re->TextureBind( m_texCube );
		m_re->VerticesDraw( m_cubeParts );
	}

	//draw sprite particles
	//if ( 0 )
	{
		uint	spriteCount = m_psSprite.GetParticlesCount();
		if ( spriteCount > MAX_VB_SPRITE_PARTICLES ) spriteCount = MAX_VB_SPRITE_PARTICLES;
		m_spriteParts->SetValidVertices( spriteCount * 4 );
		m_spriteParts->SetValidIndices( spriteCount * 6 );
		
		vbVertex_t *v = m_spriteParts->GetWriteVertices();
		const particle_t *p = m_psSprite.GetParticles();
		
		nsVec3	up, right;
		for ( uint i = 0; i < spriteCount; i++, p++, v += 4 )
		{
			up = p->size * 0.5f * camUp;
			right = p->size * 0.5f * camRight;
			v[0].v = - up - right + p->pos;
			v[1].v = -up + right + p->pos;
			v[2].v = up + right + p->pos;
			v[3].v = up - right + p->pos;
			
			v[0].color = p->color.ToARGB();
			v[1].color = p->color.ToARGB();
			v[2].color = p->color.ToARGB();
			v[3].color = p->color.ToARGB();
		}
		
		m_re->SetAlphaMode( ALPHA_GLASS );
		m_re->SetZBufferState( true, false );
		m_re->TextureBind( m_texSprite );
		m_re->VerticesDraw( m_spriteParts );
	}

	
	/*m_re->SetRenderMode( RM_ORTHO_640 );
	
	m_re->TextureBind( 0 );
	m_aux->DrawSprite( 100, 100, 100, 100 );
	m_re->SetAlphaMode( ALPHA_SUBTRACT );
	m_re->TextureBind( m_texSprite );
	m_aux->DrawSprite( 100, 100, 100, 100 );//*/
	
	m_re->SceneEnd();
}

//-----------------------------------------------------
//  nsRenderScene::IsActive:  
//-----------------------------------------------------
bool nsRenderScene::IsActive()
{
	bool	active = g_mainWnd.m_bActive || g_mainWnd.m_bUpdateView;
	g_mainWnd.m_bUpdateView = false;
	return active;
}

//-----------------------------------------------------
//  nsRenderScene::LoadAPI:  
//-----------------------------------------------------
bool nsRenderScene::LoadAPI( const char *dllName )
{
	app->Log( PRN_ALL, "...loading render '%s'\n", dllName );

	try
	{
		if ( !(m_renderDll = LoadLibrary( dllName )) )
			throw StrPrintf( "Can't load render library\n" );

		GetRenderAPI_t	GetAPI = (GetRenderAPI_t)GetProcAddress( m_renderDll, "GetRenderAPI" );
		if ( !GetAPI ) throw StrPrintf( "Invalid render library\n" );

		renderImport_t	ri;
		memset( &ri, 0, sizeof(ri) );
		ri.renderVersion = RENDER_VERSION;
		ri.app = app;
		ri.cfg = cfg;
		ri.fs = fs;

		if ( !(m_re = GetAPI( &ri )) )
			throw StrPrintf( "Can't get render API\n" );
	}
	catch ( const char* error )
	{
		m_re = 0;
		if ( m_renderDll )
		{
			FreeLibrary( m_renderDll );
			m_renderDll = 0;
		}
		app->FatalError( error );
		return false;
	}

	return true;
}
