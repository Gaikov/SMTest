#ifndef _VERTEXBUFFER_H_
#define _VERTEXBUFFER_H_

#include "RLocal.h"

#define	FVF_VBVERTEX	( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

class nsVertexBuffer : public IVertexBuffer  
{
friend class nsVertexBufferManager;
friend class nsRenderExport;

public:
	nsVertexBuffer			*prev, *next;	//��� �������� ������ � ���������

private:
	IDirect3DVertexBuffer8	*m_vb;
	IDirect3DIndexBuffer8	*m_ib;
	uint					m_vertsCount, m_indexCount;
	uint					m_vertsValidCount, m_indexValidCount;
	
	vbVertex_t				*m_verts;
	word					*m_indices;
	
	bool					m_isDirtyVB;	//true - ���� ������ ������� ���� �������� � ����� �������� ������ �������
	bool					m_isDirtyIB;
	
	primitiveMode_t			m_mode;

private:
	nsVertexBuffer( IDirect3DDevice8* dev, uint vertsCount, uint indexCount );
	virtual ~nsVertexBuffer();
	bool	IsValid();
	bool	ReCreate( IDirect3DDevice8* dev, uint vertsCount, uint indexCount );
	void	Draw( IDirect3DDevice8* dev );

	void	Release();
	bool	OnDeviceLost();
	bool	OnDeviceReset( IDirect3DDevice8* dev );
	bool	UpdateBuffers();
	bool	CreateBuffers( IDirect3DDevice8* dev );

private:
	virtual void		SetValidVertices( uint count );
	virtual uint		GetValidVertices();
	
	virtual void		SetValidIndices( uint count );
	virtual uint		GetValidIndices();
	
	virtual void		SetPrimitiveMode( primitiveMode_t mode );
	
	virtual uint		GetVerticesCount();
	virtual uint		GetIndicesCount();
	
	virtual vbVertex_t*	GetReadVertices();
	virtual vbVertex_t*	GetWriteVertices();
	
	virtual word*		GetReadIndices();
	virtual word*		GetWriteIndices();
};

#endif
