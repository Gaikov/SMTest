#ifndef _STATES_H_
#define _STATES_H_

#include "RLocal.h"

class nsStates  
{
friend class nsRenderExport;

public:
	static nsStates*	OnCreateDevice( IDirect3DDevice8* dev, const renderConfig_t *currCfg );
	static void			OnDestroyDevice( nsStates* sb );

	static bool			BeginState( IDirect3DDevice8* dev );
	static bool			EndState( IDirect3DDevice8 *dev, DWORD &state );

private:
	IDirect3DDevice8		*m_dev;
	const renderConfig_t	*m_cfg;

	DWORD	m_sbOrtho640;
	DWORD	m_sbOrthoReal;
	DWORD	m_sbFrustrum;
	
	DWORD	m_sbAlphaGlass, m_sbAlphaAdd, m_sbAlphaMul, m_sbAlphaLightMap;
	DWORD	m_sbAlphaSubtract;
	
	DWORD	m_sbTexNearest, m_sbTexLinear;
	
	DWORD	m_sbTexNormal, m_sbTexTrans, m_sbTexProj, m_sbTexSphere;
	DWORD	m_sbTexReplace, m_sbTexModul;
	DWORD	m_sbTexWrap, m_sbTexClamp;

private:
	nsStates();
	virtual ~nsStates();

	bool	InitStates();
	void	FreeStates();

	bool	InitTexFilter();
	bool	InitRenderMode();
	bool	InitAlphaModes();
	bool	InitTexModes();

};

#endif
