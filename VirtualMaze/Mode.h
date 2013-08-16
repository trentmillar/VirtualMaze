#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>

#include <ddraw.h>


#include "Network.h"
#include "s.h"
#include "Logger.h"

typedef struct s_drawinfo_t{
	int width, height;
	void *lpSurface;
	int lPitch;
	HWND hwnd;
	PALETTEENTRY palette[256];

	// DirectDraw stuff
	DDSURFACEDESC       ddsd;
	LPDIRECTDRAW        lpDD; 
	LPDIRECTDRAWSURFACE lpDDSPrimary;
	LPDIRECTDRAWSURFACE lpDDSBack;
	LPDIRECTDRAWPALETTE lpDDPal;
}drawinfo_t;

extern drawinfo_t drawinfo;

class Mode
{
private:
	Network *network;

public:
	Mode(void);
	~Mode(void);

	void init_draw(GLuint width, GLuint height);//drawinfo_t *d);
	void term_draw(drawinfo_t *d);
	void set_pal(drawinfo_t *d, char *pal);
	BOOL lock_surf(drawinfo_t *d);
	void unlock_blit_fill(drawinfo_t *d);
	void fatal_error(const char *message, char *file, int line);
};

