#ifndef _VERTEXBUFFERMANAGER_H_
#define _VERTEXBUFFERMANAGER_H_

#include "RLocal.h"
#include "VertexBuffer.h"

class nsVertexBufferManager
{
friend class nsRenderExport;
private:
	nsVertexBuffer		*m_vbList;
	IDirect3DDevice8	*m_dev;

private:
	nsVertexBufferManager();
	virtual ~nsVertexBufferManager();

	bool			Init( IDirect3DDevice8 *dev );
	void			Release( bool userResource );

	nsVertexBuffer*	CreateVertexBuffer( uint vertsCount, uint indexCount );
	void			ReleaseVertexBuffer( nsVertexBuffer* vb );
};

#endif
