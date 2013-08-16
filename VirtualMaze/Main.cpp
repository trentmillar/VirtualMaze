#include "Game.h"

#pragma comment(lib, "Ws2_32.lib")


int WINAPI _WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int sw);
LRESULT CALLBACK _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


LRESULT CALLBACK _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_CREATE:
		ShowCursor(FALSE);
		ShowWindow(hwnd, SW_SHOWNORMAL);
		return TRUE;
	case WM_KEYDOWN:
		if (Game::nkeys < 32)
			Game::keys[31 - Game::nkeys++] = wParam;
		return 0;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI _WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int sw)
{
	int r = 0;

	HINSTANCE	hInstance;
	WNDCLASS	cls;	

	hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	cls.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	cls.lpfnWndProc		= (WNDPROC) _WndProc;					// WndProc Handles Messages
	cls.cbClsExtra		= 0;									// No Extra Window Data
	cls.cbWndExtra		= 0;									// No Extra Window Data
	cls.hInstance		= hInst;//hInstance;							// Set The Instance
	cls.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	cls.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	cls.hbrBackground	= NULL;									// No Background Required For GL
	cls.lpszMenuName		= NULL;									// We Don't Want A Menu
	cls.lpszClassName	= "VirtualMaze";

	if (!RegisterClass(&cls))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	/*
	WNDCLASS cls = {
		CS_DBLCLKS, (WNDPROC)WndProc, 0, 0, hInst,
		LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)GetStockObject(BLACK_BRUSH), NULL, L"NETMAZE"};


	// initialize window: register class and create window with it
	if (!RegisterClass(&cls))
		return 0;
	*/
	drawinfo.hwnd = CreateWindow("NETMAZE", "NETMAZE", WS_POPUP, 0, 0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN),
		NULL, NULL, hInst, NULL);
	if (!drawinfo.hwnd)
		return 0;

	Game::ParseCommandLine(szCmdLine);
	
	int p = Game::GetParameterIndex("-w");
	if(p)
	{
		Game::NetworkClientIP[0] = '\0';
		strcat(Game::NetworkClientIP, Game::Commands[p]);
	}

	//log game to file?
	if (Game::GetParameterIndex("-log"))
		Logger::IsLoggingOn = true;
	else
		Logger::IsLoggingOn = false;

	//skip rendering floor?
	if (Game::GetParameterIndex("-NoFloor"))
		Game::NoFloor = true;
	else
		Game::NoFloor = false;

	//Networking

	//check for network game
	if (Game::GetParameterIndex("-client") || Game::GetParameterIndex("-server"))
		Game::IsNetwork = true;
	else
		Game::IsNetwork = false;

	p = Game::GetParameterIndex("-client");
	if(p)
	{
		Game::NetworkClientIP[0] = '\0';
		strcat(Game::NetworkClientIP, Game::Commands[p]);
	}

	p = Game::GetParameterIndex("-port");
	if(p)
	{
		Game::NetworkPort = atoi(Game::Commands[p]);
	}
	else
	{
		Game::NetworkPort = 7500;
	}

	if(Game::GetParameterIndex("-server"))
	{
		Game::NetworkIsServer = true;
	}
	else
	{
		Game::NetworkIsServer = false;
	}


	//Game::mode->init_draw(&drawinfo);
	Game::load_graphics();

	//load maps and build trig tables
	Game::load_and_build();

	Logger::Log("\n");

	//Start network
	if (Game::IsNetwork)
	{
		if (!Game::network->StartCOM() )
			return 0;
	}

	Logger::Log("Starting raycasting...\n");

	r = Game::PlayGame();

	Game::mode->term_draw(&drawinfo);


	//Stop network
	if (Game::IsNetwork)
	{
		//if (!network->StopCOM() )
		//	return 0;
		
		Game::network->StartCOM();
	}

	return r;
}