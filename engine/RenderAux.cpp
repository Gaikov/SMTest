#include "RenderAux.h"

//-----------------------------------------------------
//  nsRenderAux::nsRenderAux:  
//-----------------------------------------------------
nsRenderAux::nsRenderAux( IRenderExport *re )
{
	assert( re != 0 );
	m_re = re;

	m_quad = m_re->VerticesCreate( 4, 6 );
	vbVertex_t	*v = m_quad->GetWriteVertices();
	v[0].v = nsVec3( 0, 0, 0 );
	v[0].tu = 0;	v[0].tv = 0;
	v[1].v = nsVec3( 0, 1, 0 );
	v[1].tu = 0;	v[1].tv = 1;
	v[2].v = nsVec3( 1, 1, 0 );
	v[2].tu = 1;	v[2].tv = 1;
	v[3].v = nsVec3( 1, 0, 0 );
	v[3].tu = 1;	v[3].tv = 0;

	// 0|----|3
	//  |\   |
	//  |  \ |
	// 1|----|2  
	
	word		*i = m_quad->GetWriteIndices();
	i[0] = 0;
	i[1] = 1;
	i[2] = 2;

	i[3] = 0;
	i[4] = 2;
	i[5] = 3;
}

//-----------------------------------------------------
//  nsRenderAux::~nsRenderAux:  
//-----------------------------------------------------
nsRenderAux::~nsRenderAux()
{

}

//-----------------------------------------------------
//  nsRenderAux::BuildCoordSystem:  
//-----------------------------------------------------
IVertexBuffer* nsRenderAux::BuildCoordSystem()
{
	IVertexBuffer	*vb = m_re->VerticesCreate( 6, 6 );
	if ( !vb ) return 0;
	
	vb->SetPrimitiveMode( PM_LINES );
	vbVertex_t	*v = vb->GetWriteVertices();
	word		*idx = vb->GetWriteIndices();
	for ( int i = 0; i < 6; i++ )
	{
		v[i].color = 0xffffffff;
		idx[i] = i;
	}
	
	v[0].v = nsVec3( 0, 0, 0 );
	v[0].color = 0xffff0000;
	v[1].v = nsVec3( 1, 0, 0 );
	v[1].color = 0xffff0000;
	
	
	v[2].v = nsVec3( 0, 0, 0 );
	v[2].color = 0xff00ff00;
	v[3].v = nsVec3( 0, 1, 0 );
	v[3].color = 0xff00ff00;
	
	v[4].v = nsVec3( 0, 0, 0 );
	v[4].color = 0xff0000ff;
	v[5].v = nsVec3( 0, 0, 1 );
	v[5].color = 0xff0000ff;
	
	return vb;
}

//-----------------------------------------------------
//  nsRenderAux::BuildWireCube:  
//-----------------------------------------------------
IVertexBuffer* nsRenderAux::BuildWireCube( float size, nsColor& color )
{
	IVertexBuffer	*vb = m_re->VerticesCreate( 8, 24 );
	if ( !vb ) return 0;

	vb->SetPrimitiveMode( PM_LINES );
	vbVertex_t	*v = vb->GetWriteVertices();
	word		*idx = vb->GetWriteIndices();
	float		s = size * 0.5f;

	//top
	v[0].v = nsVec3( -s, s, s );
	v[1].v = nsVec3( -s, s, -s );
	v[2].v = nsVec3( s, s, -s );
	v[3].v = nsVec3( s, s, s );

	//bottom
	v[4].v = nsVec3( -s, -s, s );
	v[5].v = nsVec3( -s, -s, -s );
	v[6].v = nsVec3( s, -s, -s );
	v[7].v = nsVec3( s, -s, s );

	dword	c = color.ToARGB();
	for ( int i = 0; i < 8; i++ )
		v[i].color = c;

	word	si[] = 
	{
		0, 1, 1, 2, 2, 3, 3, 0,
		1, 5, 2, 6, 3, 7, 0, 4,
		4, 5, 5, 6, 6, 7, 7, 4
	};

	memcpy( idx, si, sizeof(word) * 24 );
	return vb;
}

//-----------------------------------------------------
//  nsRenderAux::DrawSprite:  
//-----------------------------------------------------
void nsRenderAux::DrawSprite( float x, float y, float width, float height, nsColor& color )
{
	if ( !m_quad ) return;
	vbVertex_t	*v = m_quad->GetWriteVertices();

	v[0].v = nsVec3( x, y, 0 );
	v[1].v = nsVec3( x, y + height, 0 );
	v[2].v = nsVec3( x + width, y + height, 0 );
	v[3].v = nsVec3( x + width, y, 0 );

	m_re->VerticesDraw( m_quad );
}