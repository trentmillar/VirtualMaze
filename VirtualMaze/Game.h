#pragma once

#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "Mode.h"
#include "s.h"
#include "Network.h"
#include "Logger.h"

/* ------------------------------------------------------------------- */
/* Various constants */

/* Boolean */

//#define FALSE       0
//#define TRUE        1

/* Video */

#define FLOOR       243
#define CEILING     176
#define STRIP_SIZE  200
#define SCREEN_WIDTH      (unsigned int)320
#define SCREEN_HEIGHT     (unsigned int)200

#define CHAR_WIDTH        8
#define CHAR_HEIGHT       8

/* Math */

#define PI 	    3.1415926536
#define TSIZE       1920
#define ANGLEINC    ((double)360/TSIZE)
#define D90         TSIZE/120*90/3
#define D180        2*D90
#define D270        3*D90
#define INFINITY    1e6
#define NOTZERO     1e-6
#define SCALE       14000

/* Game */

#define MAPCOLS	    64
#define MAPROWS	    20
#define CELLSIZE    64
#define RMAX        MAPROWS*CELLSIZE
#define CMAX	    MAPCOLS*CELLSIZE
#define NUMWALLS    5

/* Stuff to do on-the-fly */

#define shl(r,n) ((int)r << n)
#define pp(r,c,color) vbuff[shl(r,8)+shl(r,6)+(int)c]=(unsigned char)color
#define pp2(color) {vbuff[ppos]=(unsigned char)color;ppos+=320;}
#define cellval(x,y) game_map[(int)x/CELLSIZE][(int)y/CELLSIZE]
#define sqr(z) (z)*(z)
#define is_wall(cell) ((('1' <= cell) && (cell <= '4')) || (cell == 'E'))
#define dist(x1,y1,x2,y2) sqrt(sqr(x1-x2) + sqr(y1-y2))

/* Key press */

#define UP 0x50
#define DN 0x51
#define LT 0x52
#define RT 0x53

/* ------------------------------------------------------------------- */

#define TEXT_BKG_COLOR 252

typedef struct
{
	int cellr, cellc;
	float r,c;
	float dir;
} player_type;

static player_type player;

typedef struct
{
	int cellr, cellc;
	float r,c;
	float dir;
} oppo_type;

static oppo_type player2;

//extern char host_name[256];
//extern char szBuf[256];

class Game
{
public:
	Game(void);
	~Game(void);

	static void ParseCommandLine(char *cline);
	static int GetParameterIndex(char *parm);
	static int PlayGame(void);
	static void load_graphics(void);
	static void load_and_build(void);
	static void ray_cast(void);
	static void Blit_String(int x,int y,int color, char *string);
	static void BlitScreen(char *fname);
	static void ShowStats(void);
	static int move_player(char way);

	//int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int sw);
	//static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static float hit_hwall(void);
	static float hit_vwall(void);
	static void draw_strip(int col, int size, int pos);
	static void draw_floor(int start, int col, int size);
	static void build_tables(void);
	static void load_map(void);
	static void load_walls(void);
	static void load_floor(void);
	static void load_player2(void);
	static void draw_player2(void);
	static void Blit_Char(int xc,int yc,char c,int color);
	static void PCX_Load(char *filename, unsigned char img[][64]);
	//int StartCOM(void);

	static void SetScreenWidth(unsigned int width);
	static unsigned int GetScreenWidth();

	static char Commands[10][32];

	static Mode *mode;
	static Network *network;

	static int keys[32];
	static int nkeys;

	static bool NetworkIsServer;
	static char NetworkClientIP[64];
	static int NetworkPort;
	static bool IsNetwork;
	static bool NoFloor;
	static bool IsFullScreen;
	static unsigned int ScreenHeight;

private:

	static unsigned int ScreenWidth;

	static const unsigned char char_gen_table[128][8];
	static int parameterCount;
	static int stopped, gameover;
	static time_t starttime,stoptime,seconds;
	static char transmsg[64], wbu[64], statstr[64];
	static int gotdir, has_moved;
	static char *vbuff;
	static long int frames;
	static int logged;
	static float angle,ta;
	static int tptr,slice,hslice,vslice,wall;
	static unsigned char hcolor,vcolor;

	//screen messages
	static int transptr, intrans, statmsg, waiting;
	static long int msg_die;

	/* Precomputed tables */
	static float invsin[TSIZE], invcos[TSIZE];
	static std::vector<float> corr[320];
	static std::vector<int> wd[320];

	/* Wall variables */

	static unsigned char wall_maps[NUMWALLS][CELLSIZE][CELLSIZE];

	// floor variables

	static unsigned char floorpat[CELLSIZE][CELLSIZE];
	static float fdistance,fcenter;
	static float maxdistance;
	static unsigned int ppos;

	// opponent variables

	static unsigned char oppo[CELLSIZE][CELLSIZE];

	/* Game control */

	static unsigned char game_map[MAPROWS][MAPCOLS];
	//long starttime;
	static int mousedr,mousedc;
	static unsigned char oldfloor;

	static unsigned char p1char,p2char;
};

