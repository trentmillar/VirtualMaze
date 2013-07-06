#include <windows.h>
#include <gl/gl.h>

// Defines 

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
//#define SCREEN_BIT_COLOR 32

// Function Declarations

LRESULT CALLBACK WinProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CreateGameWindow(char* title, int width, int height, /*int bits,*/ bool fullscreenflag);
void EnableGraphics();//HWND hWnd, HDC * hDC, HGLRC * hRC);
void DisableGraphics();//HWND hWnd, HDC hDC, HGLRC hRC);
inline int GetColorBitsPerPixel();

// Members

HDC			hDC=NULL;		// Private GDI Device Context
HGLRC		hRC=NULL;		// Permanent Rendering Context
HWND		hWnd=NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen=FALSE;	// Fullscreen Flag 

int screenWidth=SCREEN_WIDTH;		// Necessary for calculating ray casting, loops through all pixels and executes an x-coordinate cast
int screenHeight=SCREEN_HEIGHT;		// Necessary for calculating single height of that x's line - uses distance, screen height, and if an intersection occurred

// WinMain

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int iCmdShow)
{
	// Extract arguments out of command line
	char *cmdln = lpCmdLine;
	char *temp = (LPSTR)malloc(strlen(cmdln)+1);
	strcpy(temp, cmdln);
	char *args = strtok(temp, " ");

	while(args != NULL)
	{
		if (strcmp(args,"-f") == 0) //fullscreen
		{
			fullscreen = TRUE;
		}

		if (strcmp(args,"-w") == 0) //fullscreen
		{
			args = strtok(NULL, " ");
			screenWidth = atoi(args);
		}

		if (strcmp(args,"-h") == 0) //fullscreen
		{
			args = strtok(NULL, " ");
			screenHeight = atoi(args);
		}
		args = strtok(NULL, " ");
	}
	//WNDCLASS wc;
	MSG msg;
	BOOL quit = FALSE;
	float theta = 0.0f, scale = 1.0f;
	
	// register window class
	/*wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "VirtualMaze";
	RegisterClass( &wc );*/
	
	// create main window
	/*hWnd = CreateGameWindow( 
		"VirtualMaze", "Virtual Maze", 
		WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
		0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, NULL, hInstance, NULL );*/

	if(!CreateGameWindow("Virtual Maze", screenWidth, screenHeight, fullscreen)){
		return 0;
	}
	
	// enable OpenGL for the window
	//EnableGraphics(hWnd, &hDC, &hRC );
	
	// program main loop
	while ( !quit )
	{
		
		// check for messages
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )  )
		{
			
			// handle or dispatch messages
			if ( msg.message == WM_QUIT ) 
			{
				quit = TRUE;
			} 
			else 
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			
		} 
		else 
		{
			
			// OpenGL animation code goes here
			
			glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
			glClear( GL_COLOR_BUFFER_BIT );
			
			glPushMatrix();
			glRotatef( theta, 0.0f, 0.0f, 1.0f );
			glScalef(scale,scale,scale);
			glBegin( GL_TRIANGLES );
			glColor3f( 1.0f, 0.0f, 0.0f ); glVertex2f( 0.0f, 1.0f );
			glColor3f( 0.0f, 1.0f, 0.0f ); glVertex2f( 0.87f, -0.5f );
			glColor3f( 0.0f, 0.0f, 1.0f ); glVertex2f( -0.87f, -0.5f );			
			glEnd();
			glPopMatrix();
			
			SwapBuffers( hDC );
			
			theta += keys[VK_LEFT] ? -0.1f : keys[VK_RIGHT] ? 0.1f : 0.0f;
			scale *= keys[VK_UP] ? 1.0001f : keys[VK_DOWN] ? 0.9999f : 1.0f;
		}
		
	}
	
	// shutdown OpenGL
	DisableGraphics();// hWnd, hDC, hRC );
	
	// destroy the window explicitly
	DestroyWindow( hWnd );
	
	return msg.wParam;
	
}

// Window Procedure

LRESULT CALLBACK WinProcess(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	switch (message)
	{
		
	case WM_CREATE:
		return 0;
		
	case WM_CLOSE:
		PostQuitMessage( 0 );
		return 0;
		
	case WM_DESTROY:
		return 0;

	case WM_KEYUP:
		switch ( wParam )
		{
		default:
			keys[wParam] = FALSE;

		}
		return 0;

	case WM_KEYDOWN:
		switch ( wParam )
		{

		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;

		default:
			keys[wParam] = TRUE;

		}
		return 0;
	
	default:
		return DefWindowProc( hWnd, message, wParam, lParam );
			
	}
	
}

// Enable Graphics

void EnableGraphics()//HWND hWnd, HDC * hDC, HGLRC * hRC)
{
	PIXELFORMATDESCRIPTOR pfd;
	int format;
	
	// get the device context (DC)
	hDC = GetDC( hWnd );
	
	// set the pixel format for the DC
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = GetColorBitsPerPixel();
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	format = ChoosePixelFormat( hDC, &pfd );
	SetPixelFormat( hDC, format, &pfd );
	
	// create and enable the render context (RC)
	hRC = wglCreateContext( hDC );
	wglMakeCurrent( hDC, hRC );
	
}

// Disable Graphics

void DisableGraphics()//HWND hWnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( hRC );
	ReleaseDC( hWnd, hDC );
}

// Get color bits

inline int GetColorBitsPerPixel()
{
	
	HDC dc = GetDC(NULL);
	int bits = GetDeviceCaps(dc, BITSPIXEL);
	ReleaseDC(NULL, dc);
	return bits;
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:		
 *	title			- Title To Appear At The Top Of The Window	
 *	width			- Width Of The GL Window Or Fullscreen Mode	
 *	height			- Height Of The GL Window Or Fullscreen Mode
 *	bits			- Number Of Bits To Use For Color (8/16/24/32)	
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
BOOL CreateGameWindow(char* title, int width, int height, /*int bits,*/ bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	HINSTANCE	hInstance;				// Holds The Instance Of The Application
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values

	fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	if(fullscreen)
	{
		// Get current screen RECT           
		SetRect(&WindowRect, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	}
	else
	{
		WindowRect.left=(long)0;			// Set Left Value To 0
		WindowRect.right=(long)width;		// Set Right Value To Requested Width
		WindowRect.top=(long)0;				// Set Top Value To 0
		WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height
	}

	// Get color bits
	int bits = GetColorBitsPerPixel();
	
	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WinProcess;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "VirtualMaze";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if (fullscreen)												// Attempt Fullscreen Mode?
	{		
		int fullscreenWidth  = WindowRect.right-WindowRect.left;//GetDeviceCaps(hDC, HORZRES);
		int fullscreenHeight = WindowRect.bottom-WindowRect.top;//GetDeviceCaps(hDC, VERTRES);
		//int colourBits       = GetDeviceCaps(hDC, BITSPIXEL);
		//int refreshRate      = GetDeviceCaps(hDC, VREFRESH);

		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= fullscreenWidth;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= fullscreenHeight;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
		//dmScreenSettings.dmDisplayFrequency = refreshRate;
		dmScreenSettings.dmFields= DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;// | DM_DISPLAYFREQUENCY;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if ( MessageBox(NULL,
					"The requested full screen mode is not supported by\nyour video card. Use windowed mode instead?",
					"Virtual Maze",
					MB_YESNO|MB_ICONEXCLAMATION) == IDYES )
			{
				fullscreen=FALSE;		// Windowed Mode Selected.  fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In full screen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;						// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
		"VirtualMaze",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right-WindowRect.left,	// Calculate Window Width
		WindowRect.bottom-WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Do not Pass Anything To WM_CREATE
	{
		DisableGraphics();								// Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		32,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
	{
		DisableGraphics();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		DisableGraphics();								// Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		DisableGraphics();								// Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		DisableGraphics();								// Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
	{
		DisableGraphics();								// Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	//ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	//if (!InitGL())									// Initialize Our Newly Created GL Window
	//{
	//	DisableGraphics();								// Reset The Display
	//	MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
	//	return FALSE;								// Return FALSE
	//}
	EnableGraphics();

	return TRUE;									// Success
}
