#include "RLocal.h"
#include "RenderExport.h"
#include "RLocal.h"
#include "UtilsD3D.h"

IMainApp		*app;
IConfig			*cfg;
IFileSystem		*fs;

extern "C"
{
	__declspec( dllexport ) IRenderExport*	__cdecl GetRenderAPI( renderImport_t* import );
}

//-----------------------------------------------------
//  CreateRender:  
//-----------------------------------------------------
IRenderExport* __cdecl GetRenderAPI( renderImport_t* import )
{
	if ( !import || import->renderVersion != RENDER_VERSION ) return false;
	
	app = import->app;
	cfg = import->cfg;
	fs = import->fs;

	return nsRenderExport::Create();
}

