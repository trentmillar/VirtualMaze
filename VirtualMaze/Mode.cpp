#include "Mode.h"


Mode::Mode(void)
{
}


Mode::~Mode(void)
{
}

drawinfo_t drawinfo; // the global drawing state

// Restore surfaces lost from video memory
static HRESULT RestoreSurfaces(drawinfo_t *d){
	return IDirectDrawSurface_Restore(d->lpDDSPrimary);
}

/*void Mode::DrawLine(void) {
	glClear(GL_COLOR_BUFFER_BIT);  
	glColor3f(0.0,0.4,0.2); 
	glPointSize(3.0);  

	glBegin(GL_LINES);
	glVertex2d(180, 15);
	glVertex2d(10, 145);
	glEnd();
} */

void Mode::init_draw(GLuint width, GLuint height)//drawinfo_t *d) // initialize drawing functionality
{
	glClearColor(0, 0, 0, 0);
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, width, 0, height, 1, -1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return;
	/*
	LPDDSURFACEDESC       ddsd;
	HRESULT             ddrval;

	// create the main DirectDraw object
	ddrval = DirectDrawCreate(NULL, &d->lpDD, NULL) != DD_OK;
	if (ddrval != DD_OK)
	{
		Logger::Log("Couldn't init DirectDraw");
		Logger::Die("Couldn't init DirectDraw", __FILE__, __LINE__);
	}
	// Get exclusive mode
	ddrval = IDirectDraw_SetCooperativeLevel(d->lpDD, d->hwnd, DDSCL_EXCLUSIVE |
		DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT);
	if (ddrval != DD_OK)
	{
		Logger::Log("Couldn't set DirectDraw exclusive mode");
		Logger::Die("Couldn't set DirectDraw exclusive mode", __FILE__, __LINE__);
	}
	// Set the video mode to 320x200x8
	ddrval = IDirectDraw_SetDisplayMode(d->lpDD, 320, 200, 8);
	//IDirectDraw::SetDisplayMode(320, 200, 8,0,0);
	//ddrval=d->lpDD->SetDisplayMode(320,200,8, 0, 0);
	//ddrval= IDirectDraw_SetDisplayMode(320, 200, 8, 0, 0);
	if (ddrval != DD_OK)
	{
	Logger::Log("Couldn't set DirectDraw display mode");
	Logger::Die("Couldn't set DirectDraw display mode", __FILE__, __LINE__);
	}
	// Create the primary surface with 1 back buffer
	ddsd->dwSize = sizeof(ddsd);
	ddsd->dwFlags = DDSD_CAPS;
	ddsd->ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	ddrval = IDirectDraw2_CreateSurface(d->lpDD, ddsd, &d->lpDDSPrimary, NULL);
	if (ddrval != DD_OK)
	{
		Logger::Log("Couldn't create display surface");
		Logger::Die("Couldn't create display surface", __FILE__, __LINE__);
	}
	ddsd->dwSize = sizeof(ddsd);
	ddsd->dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd->ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ddsd->dwWidth = 320;
	ddsd->dwHeight = 200;
	ddrval = IDirectDraw2_CreateSurface(d->lpDD, ddsd, &d->lpDDSBack, NULL);
	if (ddrval != DD_OK)
	{
		Logger::Log("Couldn't create frame surface");
		Logger::Die("Couldn't create frame surface", __FILE__, __LINE__);
	}
	*/
}

void Mode::term_draw(drawinfo_t *d) // terminate all drawing support
{
	if (d->lpDD != NULL) {
		if (d->lpDDSPrimary) {
			IDirectDrawSurface_Release(d->lpDDSPrimary);
			d->lpDDSPrimary = NULL;
		}
		if (d->lpDDSBack) {
			IDirectDrawSurface_Release(d->lpDDSBack);
			d->lpDDSBack = NULL;
		}
		if (d->lpDDPal) {
			IDirectDrawPalette_Release(d->lpDDPal);
			d->lpDDPal = NULL;
		}
		IDirectDraw_Release(d->lpDD);
		d->lpDD = NULL;
	}
}

void Mode::set_pal(drawinfo_t *d, char *pal) // create and set the palette
{
	PALETTEENTRY black = {0, 0, 0, 0}, white = {255, 255, 255, 0};
	int i;

	for (i=1; i < 255; i++) {
		pal += 3;
		d->palette[i].peRed   = pal[0];
		d->palette[i].peGreen = pal[1];
		d->palette[i].peBlue  = pal[2];
		d->palette[i].peFlags = (BYTE)0;
	}
	d->palette[0] = black;
	d->palette[255] = white;

	IDirectDraw_CreatePalette(d->lpDD, DDPCAPS_8BIT, d->palette, &d->lpDDPal, NULL);
	if (d->lpDDPal)
		IDirectDrawSurface_SetPalette(d->lpDDSPrimary, d->lpDDPal);
}

BOOL Mode::lock_surf(drawinfo_t *d)
{
	HRESULT       ddrval;

	d->ddsd.dwSize = sizeof(d->ddsd);
	while (1) { // lock and get pointer from surface
		ddrval = IDirectDrawSurface2_Lock(d->lpDDSBack, NULL, &d->ddsd, DDLOCK_WAIT, NULL);

		if (ddrval == DD_OK)
			break;
		if (ddrval == DDERR_SURFACELOST) {
			ddrval = RestoreSurfaces(d);
			if (ddrval != DD_OK)
				return FALSE;
		}
		if (ddrval != DDERR_WASSTILLDRAWING)
			return FALSE;
	}

	d->lpSurface = d->ddsd.lpSurface;
	d->lPitch = d->ddsd.lPitch;
	return TRUE;
}

void Mode::unlock_blit_fill(drawinfo_t *d)
{
	HRESULT ddrval;
	RECT rc = {0, 0, 320, 200};
	DDBLTFX fx;
	fx.dwSize = sizeof(fx);
	fx.dwFillColor = 0;

	d->lpSurface = NULL; // done using surface pointer
	while (1) { // unlock the surface
		ddrval = IDirectDrawSurface_Unlock(d->lpDDSBack, d->ddsd.lpSurface);

		if (ddrval == DD_OK)
			break;
		if (ddrval == DDERR_SURFACELOST) {
			ddrval = RestoreSurfaces(d);
			if(ddrval != DD_OK)
				return;
		}
		if (ddrval != DDERR_WASSTILLDRAWING)
			return;
	}

	while (1) { // blit the DD surface
		ddrval = IDirectDrawSurface_BltFast(d->lpDDSPrimary, 0, 0,
			d->lpDDSBack, &rc,
			DDBLTFAST_NOCOLORKEY);

		if (ddrval == DD_OK)
			break;
		if (ddrval == DDERR_SURFACELOST){
			ddrval = RestoreSurfaces(d);
			if (ddrval != DD_OK)
				return;
		}
		if (ddrval != DDERR_WASSTILLDRAWING)
			return;
	}

	while (1) { // blacken the memory surface functionally
		ddrval = IDirectDrawSurface_Blt(d->lpDDSBack, &rc,
			d->lpDDSPrimary, &rc,
			DDBLT_COLORFILL, &fx);

		if (ddrval == DD_OK)
			break;
		if (ddrval == DDERR_SURFACELOST){
			ddrval = RestoreSurfaces(d);
			if (ddrval != DD_OK)
				return;
		}
		if (ddrval != DDERR_WASSTILLDRAWING)
			return;
	}
}

