#include "Application.h"
#include "RenderScene.h"
#include "Config.h"
#include "Camera.h"
#include "FileSystem.h"

nsApplication	g_mainApp;
IMainApp		*app = &g_mainApp;
float			g_frameTime = 0;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow )
{
	if ( g_mainApp.Init() )
	{
		
		MSG	msg;
		memset( &msg, 0, sizeof(msg) );
		while ( msg.message != WM_QUIT )
		{
			BOOL	gotMsg;
			
			if ( rend->IsActive() )
				gotMsg = PeekMessage( &msg, 0, 0, 0, PM_REMOVE );
			else
				gotMsg = GetMessage( &msg, 0, 0, 0 );
			
			if ( gotMsg )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else		
				g_mainApp.OnIdle();
		}
	}
	/*else
		cfg.m_SaveCfg = false;//*/
	
	g_mainApp.Release();
	return 0;
}


//-----------------------------------------------------
//  nsApplication::nsApplication:  
//-----------------------------------------------------
nsApplication::nsApplication()
{
	m_log = 0;
}

//-----------------------------------------------------
//  nsApplication::~nsApplication:  
//-----------------------------------------------------
nsApplication::~nsApplication()
{

}

//-----------------------------------------------------
//  nsApplication::Init:  
//-----------------------------------------------------
bool nsApplication::Init()
{
	LogOpen();
	if ( !nsFileSystem::Init() ) return false;
	if ( !nsConfig::Init() ) return false;
	if ( !nsRenderScene::Init() ) return false;

	m_prevTime = timeGetTime();
	return true;
}

//-----------------------------------------------------
//  nsApplication::Release:  
//-----------------------------------------------------
void nsApplication::Release()
{
	nsRenderScene::Release();
	nsConfig::Release( false );
	nsFileSystem::Release();
	LogClose();
}

//-----------------------------------------------------
//  nsApplication::OnIdle:  
//-----------------------------------------------------
void nsApplication::OnIdle()
{
	DWORD	currTime = timeGetTime();
	g_frameTime = float(currTime - m_prevTime) / 1000.0f;
	//Log( PRN_ALL, "frametime: %f\n", g_frameTime );

	g_mainCam.Move();
	
	rend->Loop();
	rend->DrawWorld();
	m_prevTime = currTime;
}

//-----------------------------------------------------
//  nsApplication::Log:  
//-----------------------------------------------------
void __cdecl nsApplication::Log( int prnLevel, const char *fmt, ... )
{
	va_list	list;
	char	msg[MAX_LOG_MSG];
	
	if ( m_log )
	{
		va_start( list, fmt );
		vsprintf( msg, fmt, list );
		va_end( list );
		fprintf( m_log, msg );
		fflush( m_log );
	}
}

//-----------------------------------------------------
//  nsApplication::FatalError:  
//-----------------------------------------------------
void __cdecl nsApplication::FatalError( const char *fmt, ... )
{
	va_list	list;
	char	msg[MAX_LOG_MSG];
	
	va_start( list, fmt );
	vsprintf( msg, fmt, list );
	va_end( list );
	
	Log( PRN_ALL, "FATAL ERROR: %s\n", msg );
	MessageBox( GetActiveWindow(), msg, APP_NAME, MB_OK | MB_ICONSTOP );
	
	Exit();
}

//-----------------------------------------------------
//  nsApplication::Exit:  
//-----------------------------------------------------
void nsApplication::Exit()
{
	PostQuitMessage( 0 );
}

//-----------------------------------------------------
//  nsApplication::LogOpen:  
//-----------------------------------------------------
bool nsApplication::LogOpen()
{
	nsString	fileName;
	fileName.Format( "%s.log", APP_NAME );

	if ( !(m_log = fopen( fileName, "w" )) )
		return false;
	
	time_t	t = time( 0 );
	fprintf( m_log, "open log: %s", ctime( &t ) );
	return true;
}

//-----------------------------------------------------
//  nsApplication::LogClose:  
//-----------------------------------------------------
void nsApplication::LogClose()
{
	if ( m_log )
	{
		time_t	t = time( 0 );
		fprintf( m_log, "close log: %s", ctime( &t ) );
		fclose( m_log );
		m_log = 0;
	}
}

