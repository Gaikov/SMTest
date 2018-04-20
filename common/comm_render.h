#ifndef _COMM_RENDER_H_
#define _COMM_RENDER_H_

#include "common.h"

#define		RENDER_VERSION	0x2000

//SetAlphaMode
typedef enum
{
	ALPHA_OFF,		//нет альфа наложения
	ALPHA_GLASS,	//альфа наложения как стекло
	ALPHA_ADD,		//добавления цвета источника к цвету приемника
	ALPHA_MUL,		//умножение цвета источника с цветом приемника
	ALPHA_LIGHTMAP,	//
	ALPHA_SUBTRACT	//
}
alphaMode_t;

//SetRenderMode
typedef enum
{
	RM_ORTHO_640,   //no Z buffer, no Lights
	RM_ORTHO_REAL,	//real screen dim 1:1
	RM_FRUSTUM,		//3d view
	RM_NATIVE
}
renderMode_t;

//texture format
typedef enum
{
	TF_RGB,
	TF_RGBA
}
textureFormat_t;

typedef struct 
{
	uint	width;
	uint	height;
	uint	refresh;
}
vidModeDesc_t;

typedef struct 
{
	vidModeDesc_t	*vidModes[3];
	uint			vidModesCount;
	uint			currVidMode;

	bool			fullScreen;

	bool			capsTexGen;
}
renderConfig_t;

//clear flags
#define	CLEAR_ZBUFF	1
#define CLEAR_CBUFF	2 

//веришна буфера вершин
typedef struct
{
	nsVec3	v;			//вершина
	nsVec3	n;			//нормаль
	dword	color;		//цвет вершины
	float	tu, tv;		//текстурные координаты
}
vbVertex_t; 

//режим примитивов буфера вершин
typedef enum  
{
	PM_TRIANGLES,
	PM_LINES
}
primitiveMode_t;

/*
=================================================================
	IVertexBuffer: буфер вершин
=================================================================
*/
struct IVertexBuffer
{
	virtual void		SetValidVertices( uint count ) = 0;
	virtual uint		GetValidVertices() = 0;
	
	virtual void		SetValidIndices( uint count ) = 0;
	virtual uint		GetValidIndices() = 0;
	
	virtual void		SetPrimitiveMode( primitiveMode_t mode ) = 0;
	
	virtual uint		GetVerticesCount() = 0;
	virtual uint		GetIndicesCount() = 0;
	
	//работа с вершинами
	virtual vbVertex_t*	GetReadVertices() = 0;	//для чтения
	virtual vbVertex_t*	GetWriteVertices() = 0;	//! только для записи
	
	//работа с индексами
	virtual word*		GetReadIndices() = 0;	//для чтения
	virtual word*		GetWriteIndices() = 0;	//! только для записи
};

/*
=================================================================
	ITexture:
=================================================================
*/
struct ITexture
{
	virtual void GetSize( int& width, int& height ) = 0;
	virtual void GetPicSize( int& width, int& height ) = 0;
};

/*
=================================================================
	IRenderExport:
=================================================================
*/
struct IRenderExport
{
	virtual bool					Init( uint hMainWnd ) = 0;
	virtual void					Release() = 0;
	//virtual void					ShowConfigDialog( uint hParentWnd ) = 0;
	virtual const renderConfig_t*	GetCurrentConfig() = 0;

	//textures
	virtual ITexture*		TextureLoad( const char* fileName, bool mipMap, textureFormat_t fmt ) = 0;
	virtual ITexture*		TextureCreate( const byte* data, int width, int height, bool mipMap, textureFormat_t fmt ) = 0;
	virtual void			TextureRelease( ITexture* tex ) = 0;
	virtual void			TextureBind( ITexture* tex ) = 0;

	//render states
	virtual	void			SetAlphaMode( alphaMode_t mode ) = 0;
	virtual void			SetRenderMode( renderMode_t mode ) = 0;
	virtual void			SetZBufferState( bool test, bool write ) = 0;
	virtual void			SetWorldMatrix( nsMatrix& m ) = 0;

	//scene
	virtual bool			SceneBegin( nsVec3& camPos, nsVec3& camAt, nsVec3& camUp ) = 0;
	virtual void			SceneClear( uint flags ) = 0;
	virtual void			SceneEnd() = 0;
	virtual void			GetInvertView( nsMatrix& m ) = 0;
	virtual void			GetView( nsMatrix& m ) = 0;
	virtual nsVec3			ProjectVector( nsVec3& v ) = 0;

	//
	virtual IVertexBuffer*	VerticesCreate( uint vertsCount, uint indexCount ) = 0;
	virtual	void			VerticesRelease( IVertexBuffer* vb ) = 0;
	virtual	void			VerticesDraw( IVertexBuffer* vb ) = 0;

	//lighting
	virtual void			Lighting( bool enable ) = 0;
	virtual void			LightEnable( int source, nsVec3& dir, nsColor& c ) = 0;
	virtual void			LightDisable( int source ) = 0;
	virtual void			LightAmbient( nsColor& c ) = 0;
};

typedef struct
{
	int			renderVersion;	//должна быть установлена в RENDER_VERSION
	IMainApp	*app;
	IConfig		*cfg;
	IFileSystem	*fs;
}
renderImport_t;

#define GET_RENDER_FUNC	"GetRenderAPI"

typedef	IRenderExport*	(__cdecl *GetRenderAPI_t)( renderImport_t* import );

#endif