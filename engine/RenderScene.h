#ifndef _RENDERSCENE_H_
#define _RENDERSCENE_H_

#include "elocal.h"
#include "RenderAux.h"
#include "ParticleSystem.h"

#define	MAX_SPRITE_PARTICLES	3000
#define MAX_CUB_PARTICLES		1000

class nsRenderScene
{
friend class nsApplication;

public:
	bool	IsActive();

private:
	nsRenderScene();
	virtual ~nsRenderScene();

	static bool Init();
	static void Release();

	bool		OnInit();
	void		OnRelease();
	
	void		Loop();
	void		DrawWorld();

private:
	HMODULE			m_renderDll;
	IRenderExport	*m_re;
	nsRenderAux		*m_aux;
	
	IVertexBuffer	*m_coordSys;
	IVertexBuffer	*m_wireCube;
	IVertexBuffer	*m_spriteParts;
	IVertexBuffer	*m_cubeParts;

	ITexture		*m_texSprite, *m_texCube;
	ITexture		*m_texTest;

	nsParticleSystem<MAX_SPRITE_PARTICLES, nsSpriteInit, nsSpriteMove, true>	m_psSprite;
	nsParticleSystem<MAX_CUB_PARTICLES, nsCubeInit, nsCubeMove, false>			m_psCube;

private:
	bool LoadAPI( const char* dllName );
};

extern	bool	g_drawSpriteParts;
extern	bool	g_drawCubeParts;

extern	nsRenderScene	*rend;

#endif
