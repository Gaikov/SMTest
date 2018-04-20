#ifndef _ENUMD3D_H_
#define _ENUMD3D_H_

#include "RLocal.h"

typedef struct
{
	HRESULT	code;
	char	*desc;
}
errorDesc_t;

typedef struct
{
	D3DFORMAT	fmt;
	int			bits;
	int			components;
	char		*d3dDesc;
	char		*fullDesc;
}
formatDesc_t;

//-----------------------------------------------------
//  class nsUtilsD3D:  
//-----------------------------------------------------
class nsUtilsD3D
{
public:
	static bool				Create();
	static void				Release();

	IDirect3D8*				GetD3D() { return m_d3d; }
	bool					SetCurrentDevice( UINT devID, D3DDEVTYPE devType );

	const char*				GetErrorDesc( HRESULT hr ) const;
	const formatDesc_t*		GetFormatDesc( D3DFORMAT fmt ) const;

	void					FatalError( const char* funcName, HRESULT hr );

	int						FindUserMode( uint width, uint height );//-1 not found
	const vidModeDesc_t*	GetModeDesc( int index );				//NULL not present

	D3DFORMAT				FindFSBackBufferFormat( int colorBits );
	bool					CheckWinBackBufferFormat( D3DFORMAT fmt );

	D3DFORMAT				GetTextureFormat( int texbits, D3DFORMAT backBufferFmt );
	D3DFORMAT				GetTextureFormatRGB( int texBits, D3DFORMAT backBufferFmt );

private:
	IDirect3D8					*m_d3d;
	UINT						m_devID;
	D3DDEVTYPE					m_devType;

	vidModeDesc_t				*m_modes;
	int							m_modesCount;
	
	static const errorDesc_t	m_errorDesc[];
	static const int			m_errorCount;

	static const formatDesc_t	m_fmtDesc[];
	static const int			m_fmtCount;

private:
	nsUtilsD3D();
	virtual ~nsUtilsD3D();

	bool	EnumModes();
};

extern	nsUtilsD3D	*utd3d;

inline v3todx( D3DXVECTOR3& d, nsVec3& s )
{
	d.x = s.x;
	d.y = s.y;
	d.z = s.z;
}

#endif
