#ifndef _RLOCAL_H_
#define _RLOCAL_H_

#include <windows.h>
#include <d3d8.h>
#include <d3dx8.h>
#include <assert.h>
#include "../common/comm_render.h"

extern	IMainApp	*app;
extern	IConfig		*cfg;
extern	IFileSystem	*fs;

extern	IGameVar	*r_fullscreen;
extern	IGameVar	*r_pixel_bits;
extern	IGameVar	*r_tex_bits;
extern	IGameVar	*r_tex_filter;
extern	IGameVar	*r_vsync;
extern	IGameVar	*r_brightness;
extern	IGameVar	*r_d3d_device;
extern	IGameVar	*r_width, *r_height;
extern	IGameVar	*r_refresh;

#endif