#ifndef _RENDERAUX_H_
#define _RENDERAUX_H_

#include "elocal.h"

class nsRenderAux
{
friend class nsRenderScene;

private:
	IRenderExport	*m_re;
	IVertexBuffer	*m_quad;	//для рисования спрайтов

private:
	IVertexBuffer*	BuildCoordSystem();
	IVertexBuffer*	BuildWireCube( float size, nsColor& color = nsColor( 1, 1, 1, 1 ) );
	void			DrawSprite( float x, float y, float width, float height, nsColor& color = nsColor() );

	nsRenderAux( IRenderExport *re );
	virtual ~nsRenderAux();
};

#endif
