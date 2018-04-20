#ifndef _ELOCAL_H_
#define _ELOCAL_H_

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include "../nsLib/nsLib.h"
#include "../common/common.h"
#include "../common/comm_render.h"


extern	IMainApp			*app;
extern	IRenderExport		*render;

#define log_printf	app->Log

extern	float	g_frameTime;
extern	bool	g_moveWorld;

#endif