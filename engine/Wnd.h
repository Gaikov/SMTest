#ifndef _WND_H_
#define _WND_H_

#include "elocal.h"

class nsMainWnd
{
	friend class nsRenderScene;

public:
	nsMainWnd();
	~nsMainWnd();
	
	bool			Create( const char *title, const char *classname );
	bool			Destroy();
	virtual LRESULT WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	void			ClientToRender( int x, int y, float &rx, float &ry );
	void			MinimizeWindow();

protected:
	static LRESULT CALLBACK StaticWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	
	void	OnActivateApp( bool active );

protected:
	bool	m_bActive;
	bool	m_bUpdateView;
	bool	m_bFullScreen;
	HWND	m_hWnd;
};

extern nsMainWnd	g_mainWnd;

#endif 
