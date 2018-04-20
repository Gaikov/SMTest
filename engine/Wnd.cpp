#include "elocal.h"
#include "Wnd.h"
#include "RenderScene.h"

nsMainWnd	g_mainWnd;

#define WIN_STYLE	(WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX)

//-----------------------------------------------------
//  nsMainWnd::nsMainWnd:  
//-----------------------------------------------------
nsMainWnd::nsMainWnd()
{
	m_hWnd = 0;
	m_bUpdateView = false;
	m_bFullScreen = false;
}

//-----------------------------------------------------
//  nsMainWnd::~nsMainWnd:  
//-----------------------------------------------------
nsMainWnd::~nsMainWnd()
{
	m_bActive = false;
}

//-----------------------------------------------------
//  nsMainWnd::StaticWndProc:  
//-----------------------------------------------------
LRESULT CALLBACK nsMainWnd::StaticWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	nsMainWnd	*pWnd = (nsMainWnd*)GetWindowLong( hWnd, GWL_USERDATA );
	if ( pWnd )
		return pWnd->WindowProc( hWnd, uMsg, wParam, lParam );

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//-----------------------------------------------------
//  nsMainWnd::Destroy:  
//-----------------------------------------------------
bool nsMainWnd::Destroy()
{
	bool	bResult = true;
	
	if ( m_hWnd )
	{
		char	classname[256];
		GetClassName( m_hWnd, classname, sizeof(classname) );

		SetWindowLong( m_hWnd, GWL_WNDPROC, (long)DefWindowProc );

		log_printf( PRN_ALL, "...destroy window\n" );
		if ( !DestroyWindow( m_hWnd ) )
		{
			log_printf( PRN_ALL, "WARNING: destroy window\n" );
			bResult = false;
		}
		m_hWnd = 0;

		log_printf( PRN_ALL, "...unregister class\n" );
		if ( !UnregisterClass( classname, GetModuleHandle( 0 ) ) )
		{
			log_printf( PRN_ALL, "WARNING: unregister class\n" );
			bResult = false;
		}
	}

	return bResult;
}

//-----------------------------------------------------
//  nsMainWnd::Create:  
//-----------------------------------------------------
bool nsMainWnd::Create( const char *title, const char *classname )
{
	WNDCLASS	wc;
	memset( &wc, 0, sizeof(wc) );
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hCursor = LoadCursor( 0, IDC_ARROW );
	wc.hInstance = GetModuleHandle( 0 );
	wc.lpfnWndProc = StaticWndProc;
	wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wc.lpszClassName = classname;
	wc.hIcon = 0;//LoadIcon( GetModuleHandle( 0 ), MAKEINTRESOURCE( IDI_MAIN_ICON ) );
		
	log_printf( PRN_ALL, "...register class\n" );
	if ( !RegisterClass( &wc ) )
	{
		log_printf( PRN_ALL, "ERROR: register class" );
		return false;
	}

	log_printf( PRN_ALL, "...create window\n" );
	if ( !(m_hWnd = CreateWindowEx( 0, classname, title, WIN_STYLE, 
		//CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
		100, 100, 640, 480,
		0, 0, GetModuleHandle( 0 ), 0 )) )
	{
		app->FatalError( "ERROR: create window\n" );
		return false;
	}
	SetWindowLong( m_hWnd, GWL_USERDATA, (long)this );

	return true;
}

//-----------------------------------------------------
//  nsMainWnd::WindowProc:  
//-----------------------------------------------------
LRESULT nsMainWnd::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if ( uMsg == WM_CLOSE )
	{
		app->Exit();
		return 0;
	}

	if ( uMsg == WM_SYSCOMMAND )
	{
		if ( wParam == SC_KEYMENU )
			return 0;
	}

	if ( uMsg == WM_SIZE )
	{
		OnActivateApp( !( SIZE_MAXHIDE == wParam || SIZE_MINIMIZED == wParam ) );
		return 0;
	}
 
	if ( uMsg == WM_DISPLAYCHANGE )
	{
		m_bUpdateView = true;
		return 0;
	}

	if ( uMsg == WM_ACTIVATEAPP )
	{
		OnActivateApp( wParam != 0 );
		return 0;
	}

	if ( uMsg == WM_KEYDOWN )
	{
		switch ( wParam )
		{
		case '1':
			g_drawCubeParts = !g_drawCubeParts;
			break;
		case '2':
			g_drawSpriteParts = !g_drawSpriteParts;
			break;
		}
	}

	/*if ( uMsg == WM_KEYDOWN && wParam == 0xc0 )
	{
		con.Toggle();
		return 0;
	}
	else
	{
		IUserInput	*ui = con.IsActive() ? &con : ge.GetGameUI();
		if ( ui )
		{
			float	x, y;
			switch ( uMsg )
			{
			case WM_MOUSEMOVE:
				ClientToRender( LOWORD(lParam), HIWORD(lParam), x, y );
				ui->OnMouseMove( x, y );
				return 0;
				
			case WM_CHAR:
				ui->OnChar( wParam );
				return 0;
				
			case WM_LBUTTONDOWN:
				ClientToRender( LOWORD(lParam), HIWORD(lParam), x, y );
				ui->OnLButtonDown( x, y );
				SetCapture( m_hWnd );
				return 0;
				
			case WM_LBUTTONUP:
				ClientToRender( LOWORD(lParam), HIWORD(lParam), x, y );
				ui->OnLButtonUp( x, y );
				ReleaseCapture();
				return 0;
				
			case WM_KEYDOWN:
				if ( wParam == 0xc0 && con.IsActive() )
					con.Activate( false );
				else
				{
					ui->OnKeyDown( wParam, ((lParam >> 30) & 1) == 1 );
				}
				return 0;
				
			case WM_KEYUP:
				ui->OnKeyUp( wParam );
				return 0;
			}
		}
	}//*/

	/*if ( uMsg == WM_ENTERSIZEMOVE || uMsg == WM_ENTERMENULOOP )
	{
		prevTime = timeGetTime();
		return 0;
	}
	else if ( uMsg == WM_EXITSIZEMOVE || uMsg == WM_EXITMENULOOP )
	{
		prevTime = timeGetTime();
		return 0;
	}//*/

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//-----------------------------------------------------
//  nsMainWnd::ClientToRender:  
//-----------------------------------------------------
void nsMainWnd::ClientToRender(int x, int y, float &rx, float &ry)
{
	RECT	rect;
	
	if ( m_bFullScreen )
	{
		POINT	pt;
		GetCursorPos( &pt );
		x = pt.x;
		y = pt.y;

		rect.right = GetSystemMetrics( SM_CXSCREEN );
		rect.bottom = GetSystemMetrics( SM_CYSCREEN );
	}
	else
		GetClientRect( m_hWnd, &rect );

	rx = 640.0f * float(x) / float(rect.right);
	ry = 480.0f * float(rect.bottom - y) / float(rect.bottom);
}

//-----------------------------------------------------
//  nsMainWnd::OnActivateApp:  
//-----------------------------------------------------
void nsMainWnd::OnActivateApp(bool active)
{
	log_printf( PRN_ALL, "window activate: %s\n", active ? "active" : "inactive" );

	m_bActive = active;
}

//-----------------------------------------------------
//  nsMainWnd::MinimizeWindow:  
//-----------------------------------------------------
void nsMainWnd::MinimizeWindow()
{
	PostMessage( m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0 );
}

