#include "Game.h"

int Game::parameterCount;
int Game::keys[32];
int Game::nkeys;
char Game::Commands[10][32];

bool Game::NetworkIsServer;
char Game::NetworkClientIP[64];
int Game::NetworkPort;
bool Game::IsNetwork = false;
bool Game::NoFloor;
unsigned int Game::ScreenWidth = SCREEN_WIDTH;
unsigned int Game::ScreenHeight = SCREEN_HEIGHT;
bool Game::IsFullScreen;

time_t Game::starttime;
time_t Game::stoptime;
time_t Game::seconds;
char Game::transmsg[64];
char Game::wbu[64];
char Game::statstr[64];
int Game::gotdir;
int Game::has_moved;
char *Game::vbuff;
int Game::transptr;
int Game::intrans;
int Game::statmsg;
int Game::waiting;
int Game::stopped;
int Game::gameover;
long int Game::msg_die;
long int Game::frames;

int Game::logged;
float Game::angle;
float Game::ta;
int Game::tptr;
int Game::slice;
int Game::hslice;
int Game::vslice;
int Game::wall;

unsigned char Game::hcolor;
unsigned char Game::vcolor;

float Game::invsin[TSIZE];
float Game::invcos[TSIZE];
std::vector<float> Game::corr[SCREEN_WIDTH];
std::vector<int> Game::wd[SCREEN_WIDTH];
unsigned char Game::wall_maps[NUMWALLS][CELLSIZE][CELLSIZE];
unsigned char Game::floorpat[CELLSIZE][CELLSIZE];
float Game::fdistance;
float Game::fcenter;
float Game::maxdistance;
unsigned int Game::ppos;

// opponent variables

unsigned char Game::oppo[CELLSIZE][CELLSIZE];

/* Game control */

unsigned char Game::game_map[MAPROWS][MAPCOLS];
//long starttime;

int Game::mousedr;
int Game::mousedc;

unsigned char Game::oldfloor;
unsigned char Game::p1char;
unsigned char Game::p2char;

Network *Game::network;
Mode *Game::mode;

const unsigned char Game::char_gen_table[128][8] = 
{
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x7E, 0x81, 0xA5, 0x81, 0xBD, 0x99, 0x81, 0x7E},
	{0x7E, 0xFF, 0xDB, 0xFF, 0xC3, 0xE7, 0xFF, 0x7E},
	{0x6C, 0xFE, 0xFE, 0xFE, 0x7C, 0x38, 0x10, 0x00},
	{0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x10, 0x00},
	{0x38, 0x7C, 0x38, 0xFE, 0xFE, 0x7C, 0x38, 0x7C},
	{0x10, 0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x7C},
	{0x00, 0x00, 0x18, 0x3C, 0x3C, 0x18, 0x00, 0x00},
	{0xFF, 0xFF, 0xE7, 0xC3, 0xC3, 0xE7, 0xFF, 0xFF},
	{0x00, 0x3C, 0x66, 0x42, 0x42, 0x66, 0x3C, 0x00},
	{0xFF, 0xC3, 0x99, 0xBD, 0xBD, 0x99, 0xC3, 0xFF},
	{0x0F, 0x07, 0x0F, 0x7D, 0xCC, 0xCC, 0xCC, 0x78},
	{0x3C, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x7E, 0x18},
	{0x3F, 0x33, 0x3F, 0x30, 0x30, 0x70, 0xF0, 0xE0},
	{0x7F, 0x63, 0x7F, 0x63, 0x63, 0x67, 0xE6, 0xC0},
	{0x99, 0x5A, 0x3C, 0xE7, 0xE7, 0x3C, 0x5A, 0x99},
	{0x80, 0xE0, 0xF8, 0xFE, 0xF8, 0xE0, 0x80, 0x00},
	{0x02, 0x0E, 0x3E, 0xFE, 0x3E, 0x0E, 0x02, 0x00},
	{0x18, 0x3C, 0x7E, 0x18, 0x18, 0x7E, 0x3C, 0x18},
	{0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x66, 0x00},
	{0x7F, 0xDB, 0xDB, 0x7B, 0x1B, 0x1B, 0x1B, 0x00},
	{0x3E, 0x63, 0x38, 0x6C, 0x6C, 0x38, 0xCC, 0x78},
	{0x00, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x7E, 0x00},
	{0x18, 0x3C, 0x7E, 0x18, 0x7E, 0x3C, 0x18, 0xFF},
	{0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x00},
	{0x18, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x00},
	{0x00, 0x18, 0x0C, 0xFE, 0x0C, 0x18, 0x00, 0x00},
	{0x00, 0x30, 0x60, 0xFE, 0x60, 0x30, 0x00, 0x00},
	{0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xFE, 0x00, 0x00},
	{0x00, 0x24, 0x66, 0xFF, 0x66, 0x24, 0x00, 0x00},
	{0x00, 0x18, 0x3C, 0x7E, 0xFF, 0xFF, 0x00, 0x00},
	{0x00, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x30, 0x78, 0x78, 0x78, 0x30, 0x00, 0x30, 0x00},
	{0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x6C, 0x6C, 0xFE, 0x6C, 0xFE, 0x6C, 0x6C, 0x00},
	{0x30, 0x7C, 0xC0, 0x78, 0x0C, 0xF8, 0x30, 0x00},
	{0x00, 0xC6, 0xCC, 0x18, 0x30, 0x66, 0xC6, 0x00},
	{0x38, 0x6C, 0x38, 0x76, 0xDC, 0xCC, 0x76, 0x00},
	{0x60, 0x60, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00},
	{0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00},
	{0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00},
	{0x00, 0x30, 0x30, 0xFC, 0x30, 0x30, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x60},
	{0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00},
	{0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00},
	{0x7C, 0xC6, 0xCE, 0xDE, 0xF6, 0xE6, 0x7C, 0x00},
	{0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xFC, 0x00},
	{0x78, 0xCC, 0x0C, 0x38, 0x60, 0xCC, 0xFC, 0x00},
	{0x78, 0xCC, 0x0C, 0x38, 0x0C, 0xCC, 0x78, 0x00},
	{0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x1E, 0x00},
	{0xFC, 0xC0, 0xF8, 0x0C, 0x0C, 0xCC, 0x78, 0x00},
	{0x38, 0x60, 0xC0, 0xF8, 0xCC, 0xCC, 0x78, 0x00},
	{0xFC, 0xCC, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x00},
	{0x78, 0xCC, 0xCC, 0x78, 0xCC, 0xCC, 0x78, 0x00},
	{0x78, 0xCC, 0xCC, 0x7C, 0x0C, 0x18, 0x70, 0x00},
	{0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00},
	{0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x60},
	{0x18, 0x30, 0x60, 0xC0, 0x60, 0x30, 0x18, 0x00},
	{0x00, 0x00, 0xFC, 0x00, 0x00, 0xFC, 0x00, 0x00},
	{0x60, 0x30, 0x18, 0x0C, 0x18, 0x30, 0x60, 0x00},
	{0x78, 0xCC, 0x0C, 0x18, 0x30, 0x00, 0x30, 0x00},
	{0x7C, 0xC6, 0xDE, 0xDE, 0xDE, 0xC0, 0x78, 0x00},
	{0x30, 0x78, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0x00},
	{0xFC, 0x66, 0x66, 0x7C, 0x66, 0x66, 0xFC, 0x00},
	{0x3C, 0x66, 0xC0, 0xC0, 0xC0, 0x66, 0x3C, 0x00},
	{0xF8, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00},
	{0x7E, 0x60, 0x60, 0x78, 0x60, 0x60, 0x7E, 0x00},
	{0x7E, 0x60, 0x60, 0x78, 0x60, 0x60, 0x60, 0x00},
	{0x3C, 0x66, 0xC0, 0xC0, 0xCE, 0x66, 0x3E, 0x00},
	{0xCC, 0xCC, 0xCC, 0xFC, 0xCC, 0xCC, 0xCC, 0x00},
	{0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00},
	{0x1E, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78, 0x00},
	{0xE6, 0x66, 0x6C, 0x78, 0x6C, 0x66, 0xE6, 0x00},
	{0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x00},
	{0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0x00},
	{0xC6, 0xE6, 0xF6, 0xDE, 0xCE, 0xC6, 0xC6, 0x00},
	{0x38, 0x6C, 0xC6, 0xC6, 0xC6, 0x6C, 0x38, 0x00},
	{0xFC, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xF0, 0x00},
	{0x78, 0xCC, 0xCC, 0xCC, 0xDC, 0x78, 0x1C, 0x00},
	{0xFC, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0xE6, 0x00},
	{0x78, 0xCC, 0xE0, 0x70, 0x1C, 0xCC, 0x78, 0x00},
	{0xFC, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00},
	{0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xFC, 0x00},
	{0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00},
	{0xC6, 0xC6, 0xC6, 0xD6, 0xFE, 0xEE, 0xC6, 0x00},
	{0xC6, 0xC6, 0x6C, 0x38, 0x38, 0x6C, 0xC6, 0x00},
	{0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x30, 0x78, 0x00},
	{0xFE, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xFE, 0x00},
	{0x78, 0x60, 0x60, 0x60, 0x60, 0x60, 0x78, 0x00},
	{0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02, 0x00},
	{0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x00},
	{0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF},
	{0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0x76, 0x00},
	{0xE0, 0x60, 0x60, 0x7C, 0x66, 0x66, 0xDC, 0x00},
	{0x00, 0x00, 0x78, 0xCC, 0xC0, 0xCC, 0x78, 0x00},
	{0x1C, 0x0C, 0x0C, 0x7C, 0xCC, 0xCC, 0x76, 0x00},
	{0x00, 0x00, 0x78, 0xCC, 0xFC, 0xC0, 0x78, 0x00},
	{0x38, 0x6C, 0x60, 0xF0, 0x60, 0x60, 0xF0, 0x00},
	{0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8},
	{0xE0, 0x60, 0x6C, 0x76, 0x66, 0x66, 0xE6, 0x00},
	{0x30, 0x00, 0x70, 0x30, 0x30, 0x30, 0x78, 0x00},
	{0x0C, 0x00, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0x78},
	{0xE0, 0x60, 0x66, 0x6C, 0x78, 0x6C, 0xE6, 0x00},
	{0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00},
	{0x00, 0x00, 0xCC, 0xFE, 0xFE, 0xD6, 0xC6, 0x00},
	{0x00, 0x00, 0xF8, 0xCC, 0xCC, 0xCC, 0xCC, 0x00},
	{0x00, 0x00, 0x78, 0xCC, 0xCC, 0xCC, 0x78, 0x00},
	{0x00, 0x00, 0xDC, 0x66, 0x66, 0x7C, 0x60, 0xF0},
	{0x00, 0x00, 0x76, 0xCC, 0xCC, 0x7C, 0x0C, 0x1E},
	{0x00, 0x00, 0xDC, 0x76, 0x66, 0x60, 0xF0, 0x00},
	{0x00, 0x00, 0x7C, 0xC0, 0x78, 0x0C, 0xF8, 0x00},
	{0x10, 0x30, 0x7C, 0x30, 0x30, 0x34, 0x18, 0x00},
	{0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00},
	{0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x78, 0x30, 0x00},
	{0x00, 0x00, 0xC6, 0xD6, 0xFE, 0xFE, 0x6C, 0x00},
	{0x00, 0x00, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0x00},
	{0x00, 0x00, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0xF8},
	{0x00, 0x00, 0xFC, 0x98, 0x30, 0x64, 0xFC, 0x00},
	{0x1C, 0x30, 0x30, 0xE0, 0x30, 0x30, 0x1C, 0x00},
	{0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00},
	{0xE0, 0x30, 0x30, 0x1C, 0x30, 0x30, 0xE0, 0x00},
	{0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0x00}
};

Game::Game(void)
{
	 maxdistance = 0.0;
	 network = new Network(NetworkIsServer, NetworkClientIP, NetworkPort);

}


Game::~Game(void)
{
}



int Game::PlayGame(void)
{
	MSG msg;

	int c,k,gotpkt,finload;
	static int pp;
	char playerinfo[63];
	char testm[64];

	stopped = FALSE;
	finload = FALSE;

	time(&starttime);

	for (;;) {
		// Poll messages from event queue
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				PostQuitMessage(msg.wParam);
				return msg.wParam;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//update display

		if (mode->lock_surf(&drawinfo)) 
		//{
			vbuff = reinterpret_cast<char*>(drawinfo.lpSurface);//drawinfo.lpSurface;
		//	if (!gameover)
		//		ray_cast();
		//	else
		//		if (!finload)
		//		{
		//			BlitScreen("finale");
		//			Blit_String(160 - 4 * strlen(statstr),94,190,statstr);
		//			finload = TRUE;
		//		}
		//		mode->unlock_blit_fill(&drawinfo);
		//}

		if (!gameover)
		{
			ray_cast();	
		}
		else
		{
			if (!finload)
			{
				BlitScreen("finale");
				Blit_String(160 - 4 * strlen(statstr),94,190,statstr);
				finload = TRUE;
			}
		}

		//process keystrokes

		has_moved = FALSE;
		
		while (Game::nkeys) {
			c = Game::keys[32 - Game::nkeys--];

			if (intrans)
			{
				if (c == 0x0d)
				{
					transmsg[transptr] = '\0';
					wbu[0] = '\0';
					sprintf(wbu,"Talk%s: ", network->HostName);
					strcat(wbu,transmsg);
					network->WritePacket(wbu);
					Logger::Log(wbu);
					intrans = FALSE;
				}
				else
					if (c == 0x08)
					{
						if (transptr)
						{
							transptr--;
							transmsg[transptr] = '\0';
						}
					}
					else
					{
						transmsg[transptr++] = c;
						transmsg[transptr] = '\0';
					}
			}
			else
				switch (c) {
				case 'Q':   
				case VK_ESCAPE:
					if (IsNetwork)
					{
						sprintf(playerinfo,"Exit%s",network->HostName);
						network->WritePacket(playerinfo);
					}

					if (!gameover)
						ShowStats();

					PostMessage(drawinfo.hwnd, WM_CLOSE, (WPARAM)0, (LPARAM)0);
					break;

					//rotate right
				case VK_RIGHT: move_player(RT); break;
					//and left
				case VK_LEFT: move_player(LT); break;
					//move forward
				case VK_DOWN: move_player(DN); break;
					//move backwards
				case VK_UP: move_player(UP); break;

				case VK_SPACE: break;

				case 'T':
				case 't': intrans = TRUE;
					transptr = 0;
					transmsg[0] = '\0';
					break;

				default:
					break;
			}
		}		

		if (IsNetwork)
		{
			if (has_moved)
			{
				sprintf(playerinfo,"Ppos %7.1f, %7.1f, %6.1f ",
					player.r,player.c,player.dir);
				network->WritePacket(playerinfo);
			}

			gotpkt = network->ReadPacket();

			//process packets
			Logger::Log(network->BufferSize);
			if ('P' == network->BufferSize[0])
			{
				player2.r = atof(&network->BufferSize[5]);
				player2.c = atof(&network->BufferSize[14]);
				player2.dir = atof(&network->BufferSize[23]);
			}
			else
				if ('T' == network->BufferSize[0])
				{
					statmsg = TRUE;
					statstr[0] = '\0';
					strcat(statstr,&network->BufferSize[4]);
					msg_die = frames + 30l;
					Logger::Log(network->BufferSize);
				}
				else
					if ('J' == network->BufferSize[0])
					{
						waiting = FALSE;
						strcpy(statstr,&network->BufferSize[4]);
						strcat(statstr," has joined the game");
						statmsg = TRUE;
						msg_die = frames + 30l;
						strcpy(playerinfo,"Ack");
						network->WritePacket(playerinfo);
					}
					else
						if (('E' == network->BufferSize[0]) && !waiting)
						{
							if (GetParameterIndex("-server"))
								waiting = TRUE;
							strcpy(statstr,&network->BufferSize[4]);
							strcat(statstr," has left the game");
							statmsg = TRUE;
							msg_die = frames + 30l;
						}
						else
							if ('V' == network->BufferSize[0])
							{
								strcpy(statstr,&network->BufferSize[4]);
								strcat(statstr," has won the game");
								gameover = TRUE;
								statmsg = TRUE;
								msg_die = frames + 30l;
								ShowStats();
							}
		}

		if (stopped)
			PostMessage(drawinfo.hwnd, WM_CLOSE, (WPARAM)0, (LPARAM)0);
	}
	return 0; // not reached
}

void Game::ShowStats()
{
	char testm[256];
	int oldlog;

	time(&stoptime);

	oldlog = logged;
	logged = TRUE;
	sprintf(testm,"%ld frames",frames);
	Logger::Log(testm);

	seconds = stoptime - starttime;
	sprintf(testm,"You played for %ld seconds.",
		seconds);
	Logger::Log(testm);

	sprintf(testm,"%ld frames/second",frames/seconds);
	Logger::Log(testm);

	logged = oldlog;
}

void Game::load_graphics(void)
{
	char pal[768];
	FILE *f;

	if ((f = fopen("palette.lmp", "rb")) == NULL)
	{
		Logger::Log("Couldn't read palette.lmp\n");
		Logger::Die("Couldn't read palette.lmp\n", __FILE__, __LINE__);
	}
	fread(pal, 1, 768, f);
	fclose(f);
	mode->set_pal(&drawinfo, pal); 
}


/* ------------------------------------------------------------------- */
/* Ray cast an entire scene (320 columns) */

void Game::ray_cast()
{
	float xdist,ydist,dist,temp;
	int i,r,c,ray,size;
	unsigned char color;

	color = 0;
	for(ray = 0; ray < ScreenWidth; ray++)
	{
		angle = player.dir - ray*ANGLEINC + 30;
		if (angle < 0)
			angle += 360;
		if (angle > 360)
			angle -= 360;
		ta = (angle/ANGLEINC);
		tptr = ta;
		xdist = hit_hwall();
		ydist = hit_vwall();
		if (xdist < ydist)
		{
			dist = xdist;
			color = hcolor;
			wall = color;
			slice = hslice;
		}
		else
		{
			dist = ydist;
			color = vcolor;
			wall = color;
			slice = vslice;
		}
		temp = (SCALE/(dist + NOTZERO)*corr->at(ray));
		
		wd->at(ray) = fdistance = dist;
		if (fdistance > maxdistance)
			maxdistance = fdistance;
		if (ray == ScreenWidth / 2)
			fcenter = fdistance;
		size = (int)(temp + 0.5);
		if (slice == -1) slice = 0;
		draw_strip(ray,size,slice);
	}

	frames++;

	//draw player 2;
	if (IsNetwork)
		draw_player2();

	//display local message text
	if (intrans)
		Blit_String(10,16,190,transmsg);

	//is server waiting?
	if (waiting)
	{
		Blit_String(100,100,190,"Waiting for");
		Blit_String(104,110,190,"Connection");
	}

	//display status message (remote message too)
	if (statmsg)
	{
		if (msg_die > frames)
			Blit_String(10,6,190,statstr);
		else
			statmsg = FALSE;
	}
}

/* ------------------------------------------------------------------- */
/* Look for first horizontal intersection with a wall (use invtan) */

float Game::hit_hwall()
{
	double rpos,rstep,cpos,cstep;
	int found;

	rpos = (player.cellr * CELLSIZE) - 1;
	cstep = CELLSIZE * invsin[tptr] / invcos[tptr];
	if (angle < 180)
		rstep = -CELLSIZE;
	else
	{
		rstep = CELLSIZE;
		rpos += (1+CELLSIZE);
		cstep *= -1;
	}
	cpos = player.c + (player.r - rpos) * invsin[tptr] / invcos[tptr];

	found = 0;
	while ((cpos >= 0) && (cpos < CMAX) && !found)
	{
		hcolor = cellval(rpos,cpos);
		if (is_wall(hcolor))
		{
			found = 1;
			if (hcolor == 'E')
				hcolor = NUMWALLS - 1;
			else
				hcolor -= '1';
			hslice = ((int)cpos) % CELLSIZE;
		}
		if (!found)
		{
			rpos += rstep;
			cpos += cstep;
		}
	}
	if (found)
	{
		float dtoit = (player.r - rpos) * invsin[tptr];
		if (dtoit > 0)
			return dtoit;
		else
			return -dtoit;
	}
	else
	{
		hcolor = 0;
		hslice = -1;
		return INFINITY;
	}
}

/* ------------------------------------------------------------------- */
/* Look for first vertical intersection with a wall */

float Game::hit_vwall()
{
	double rpos,rstep,cpos,cstep;
	int found;

	cpos = (player.cellc * CELLSIZE) - 1;
	rstep = CELLSIZE * invcos[tptr] / invsin[tptr];
	if ((angle >= 90) && (angle < 270))
		cstep = -CELLSIZE;
	else
	{
		cstep = CELLSIZE;
		cpos += (1+CELLSIZE);
	}
	if ((angle < 90) || (angle >= 270))
		rstep *= -1;
	rpos = player.r + (player.c - cpos) * invcos[tptr] / invsin[tptr];

	found = 0;
	while ((rpos >= 0) && (rpos < RMAX) && !found)
	{
		vcolor = cellval(rpos,cpos);
		if (is_wall(vcolor))
		{
			found = 1;
			if (vcolor == 'E')
				vcolor = NUMWALLS-1;
			else
				vcolor -= '1';
			vslice = ((int)rpos) % CELLSIZE;
		}
		if (!found)
		{
			rpos += rstep;
			cpos += cstep;
		}
	}
	if (found)
	{
		float dtoit = (player.r - rpos) * invsin[tptr];
		if (dtoit > 0)
			return dtoit;
		else
			return -dtoit;
	}
	else
	{
		vcolor = 0;
		vslice = -1;
		return INFINITY;
	}
}

/* ------------------------------------------------------------------- */
/* Draw a scaled, texture-mapped vertical strip */

void Game::draw_strip(int col, int size, int pos)
{
	int i,j,k;
	float pixel_inc,pixel_row;
	unsigned char color;

	pixel_inc = (float)CELLSIZE / size;
	if (size > STRIP_SIZE)
	{
		pixel_row = ((size - STRIP_SIZE) / 2) * pixel_inc;
		size = STRIP_SIZE;
	}
	else
		pixel_row = 0;
	j = (STRIP_SIZE - size) / 2;
	for(i = 0; i < j; i++)
		pp(i,col,CEILING);
	for(k = 0; k < size; k++)
	{
		color = wall_maps[wall][(int)pixel_row][pos];
		pixel_row += pixel_inc;
		pp(j,col,color);
		j++;
	}
	if (NoFloor)
	{
		for(i = j; i < STRIP_SIZE; i++)
			pp(i,col,FLOOR);
	}
	else
		draw_floor(j,col,STRIP_SIZE);
}
 
/* ------------------------------------------------------------------- */
/* Draw a floor strip */

void Game::draw_floor(int start, int col, int size)
{
	int j,floorspot = start,xhit,yhit;
	unsigned char floorcolor;
	while (floorspot < size)
	{
		fdistance = 7000.0/ (floorspot-100) * corr->at(col);
		xhit = -fdistance/invsin[tptr] + player.r;
		yhit = fdistance/invcos[tptr] + player.c;
		if (xhit < 0)
			xhit += 4096;
		if (yhit < 0)
			yhit += 4096;
		floorcolor = floorpat[xhit % 64][yhit % 64];
		pp(floorspot,col,floorcolor);
		floorspot++;
	}
}


void Game::PCX_Load(char *filename, unsigned char img[][64])
{
	FILE *fp;//, *fopen();
	int num_bytes,index;
	int count,isize,istep,iw,ih,x,y;
	unsigned char data;
	char fyle[12] = "";

	strcat(fyle,filename);
	strcat(fyle,".pcx");
	fp = fopen(fyle, "rb");
	for (index=0; index<128; index++)
		getc(fp);
	count=0;
	x = 0;
	y = 0;
	iw = 64;
	ih = 64;
	isize = iw * ih;
	while(count<=isize)
	{
		data = getc(fp);
		if ((data >= 192) && (data <= 255))
		{
			num_bytes = data - 192;
			data  = getc(fp);
			while(num_bytes-- > 0)
			{
				img[x][y] = data;
				count++;
				y++;
				if (y == iw)
				{
					y = 0;
					x++;
				}
			}
		}
		else
		{
			img[x][y] = data;
			count++;
			y++;
			if (y == iw)
			{
				y = 0;
				x++;
			}
		}
	}
	fclose(fp);
}

void Game::SetScreenWidth(unsigned int width)
{
	ScreenWidth = width;
	corr->resize(width);
	wd->resize(width);
}

unsigned int Game::GetScreenWidth()
{
	return ScreenWidth;
}


/* ------------------------------------------------------------------- */
/* Here we go... */

void Game::load_and_build()
{
	int k,still_playing,mins,secs;

	load_map();
	load_walls();
	load_floor();
	load_player2();
	build_tables();
}


/* ------------------------------------------------------------------- */
/* Read the game map into memory */



void Game::load_map()
{
	FILE *fp;//, *fopen();
	int r,c;
	unsigned char mdat;

	p1char = 'P';
	p2char = 'O';
	if (GetParameterIndex("-client"))
	{
		p1char = 'O';
		p2char = 'P';
	}

	fp = fopen("gamemap.dat","rb");
	for(r = 0; r < MAPROWS; r++)
	{
		for(c = 0; c < MAPCOLS; c++)
		{
			mdat = getc(fp);
			game_map[r][c] = mdat;
			if (mdat == p2char)
			{
				if (IsNetwork)
				{
					player2.cellr = r;
					player2.cellc = c;
					player2.r = r*CELLSIZE + CELLSIZE/2;
					player2.c = c*CELLSIZE + CELLSIZE/2;
					player2.dir = 0.0;
				}
				else
					game_map[r][c] = '.';

			}
			if (mdat == p1char)
			{
				player.cellr = r;
				player.cellc = c;
				player.r = r*CELLSIZE + CELLSIZE/2;
				player.c = c*CELLSIZE + CELLSIZE/2;
				player.dir = 0.0;
				oldfloor = game_map[r][c-1];
			}
		}
		mdat = getc(fp); /* eat cr */
		mdat = getc(fp); /* eat lf */
	}
	fclose(fp);
}

/* ------------------------------------------------------------------- */
/* Precompute tangent and distance-correction tables */

void Game::build_tables()
{
	int k;
	double angle;

	for(k = 0; k < TSIZE; k++)
	{
		angle = k*ANGLEINC*PI/180.0;
		switch(k)
		{
		case    0: invsin[k] = INFINITY;
			invcos[k] = 1.0;
			break;
		case  D90: invsin[k] = 1.0;
			invcos[k] = INFINITY;
			break;
		case D180: invsin[k] = -INFINITY;
			invcos[k] = -1.0;
			break;
		case D270: invsin[k] = -1.0;
			invcos[k] = INFINITY;
			break;
		default:   invsin[k] = (float)1.0/sin(angle);
			invcos[k] = (float)1.0/cos(angle);
		}
	}
	angle = -30.0*PI/180.0;
	for(k = 0; k < 320; k++)
	{
		corr->at(k) = (float)1.0/cos(angle);
		angle += 2*30/320.0*PI/180.0;
	}
}

/* ------------------------------------------------------------------- */
/* Calculate new player position based on keyboard/mouse inputs */

int Game::move_player(char way)
{
	int mr,mc,left_click,button;
	player_type temp;
	char kyb,wbu[64], wbu2[64];
	unsigned char spot;

	has_moved = TRUE;
	temp.r = player.r;
	temp.c = player.c;
	temp.dir = player.dir;
	gotdir = FALSE;
	kyb = way;
	gotdir = TRUE;
	switch(kyb)
	{
	case UP: 	temp.r -= 24*sin(temp.dir*PI/180.0);
		temp.c += 32*cos(temp.dir*PI/180.0);
		break;
	case DN: 	temp.r += 24*sin(temp.dir*PI/180.0);
		temp.c -= 32*cos(temp.dir*PI/180.0);
		break;
	case LT: 	temp.dir += 3; break;
	case RT: 	temp.dir -= 3; break;
	default: 	gotdir = FALSE;
	}
	if(temp.r < 0) temp.r = 0;
	if(temp.r > RMAX) temp.r = RMAX;
	if(temp.c < 0) temp.c = 0;
	if(temp.c > CMAX) temp.c = CMAX;
	if(temp.dir < 0)
		temp.dir += 360;
	if(temp.dir > 360)
		temp.dir -= 360;
	temp.cellr = temp.r / CELLSIZE;
	temp.cellc = temp.c / CELLSIZE;
	spot = game_map[temp.cellr][temp.cellc];
	if((spot == '.') || (spot == p1char) && gotdir)
	{
		game_map[player.cellr][player.cellc] = oldfloor;
		player.cellr = temp.cellr;
		player.cellc = temp.cellc;
		player.r = temp.r;
		player.c = temp.c;
		player.dir = temp.dir;
		oldfloor = game_map[player.cellr][player.cellc];
		game_map[player.cellr][player.cellc] = p1char;
	}
	else
		if ((spot == 'E') && gotdir)
		{
			gameover = TRUE;
			sprintf(statstr,"You have won the game");
			ShowStats();

			if (IsNetwork)
			{
				sprintf(wbu,"Vict%s",network->HostName);
				network->WritePacket(wbu);
			}

		}

		return(kyb != 'q');
}

/* ------------------------------------------------------------------- */
/* Read the wall patterns into memory */

void Game::load_walls()
{
	PCX_Load("w1",wall_maps[0]);
	PCX_Load("w2",wall_maps[1]);
	PCX_Load("w3",wall_maps[2]);
	PCX_Load("w4",wall_maps[3]);
	PCX_Load("e1",wall_maps[4]);
}

/* ------------------------------------------------------------------- */
/* Read the floor pattern into memory */

void Game::load_floor()
{
	PCX_Load("f1",floorpat);
}

void Game::load_player2()
{
	PCX_Load("obj1",oppo);
}

void Game::draw_player2()
{
	int p2col,p2colsav;
	float p2pos;
	int i,j,k,size,odist;
	float pixel_inc,pixel_row,pr;
	unsigned char color;
	float dtoit,cr,cc,dr,dc,pa,oldpa = 0.0;

	dtoit = sqrt(sqr(player2.r - player.r) + sqr(player2.c - player.c));
	odist = (int)dtoit;
	size = (SCALE/(dtoit + NOTZERO));// *corr[ray]);
	pixel_inc = (float)CELLSIZE / size;
	if (size > STRIP_SIZE)
	{
		pixel_row = ((size - STRIP_SIZE) / 2) * pixel_inc;
		size = STRIP_SIZE;
	}
	else
		pixel_row = 0;

	//find column at center
	dr = player2.r - player.r;

	dr = 0 - dr;

	dc = player2.c - player.c;
	pa = -player.dir*3.14159265/180.0;
	cc = dc*cos(pa) - dr*sin(pa);
	cr = dc*sin(pa) + dr*cos(pa);
	p2col = 160 - cr * 325.0/dtoit;
	p2colsav = p2col;

	// is object behind player?

	if (cc < 0)
		return;


	// left side first

	p2pos = 32;
	while ((p2col >= 0) && (p2col <= 319) && (p2pos >= 0))
	{
		if (odist < wd->at(p2col))
		{
			j = (STRIP_SIZE - size) / 2;
			pr = pixel_row;
			for(k = 0; k < size; k++)
			{
				color = oppo[(int)pr][(int)p2pos];
				pr += pixel_inc;
				if (color)
				{
					pp(j,p2col,color);
				}
				j++;
			}
		}
		p2col--;
		p2pos -= pixel_inc;
	}

	// now the right side

	p2col = p2colsav;
	p2pos = 32;
	while ((p2col >= 0) && (p2col <= 319) && (p2pos < 64))
	{
		if (odist < wd->at(p2col))
		{
			j = (STRIP_SIZE - size) / 2;
			pr = pixel_row;
			for(k = 0; k < size; k++)
			{
				color = oppo[(int)pr][(int)p2pos];
				pr += pixel_inc;
				if (color)
				{
					pp(j,p2col,color);
				}
				j++;
			}
		}
		p2col++;
		p2pos += pixel_inc;
	}
}

void Game::ParseCommandLine(char *commandLine)
{
	int j,k,m;
	char logMessage[64];

	j = 0;
	k = 0;
	while (k < strlen(commandLine))
	{
		if (' ' != commandLine[k])
		{
			m = 0;
			while ((' ' != commandLine[k]) && (k < strlen(commandLine)))
			{
				Commands[j][m] = commandLine[k];
				k++;
				m++;
			}
			Commands[j][m] = '\0';
			j++;
		}
		else
			k++;
	}
	parameterCount = j;

	sprintf(logMessage,"There were %d parameters\n",j);
	Logger::Log(logMessage);
	for(k = 0; k < j; k++)
	{
		sprintf(logMessage," [%d] = %s\n",k,Commands[k]);
		Logger::Log(logMessage);
	}
}

int Game::GetParameterIndex(char *key)
{
	int k;

	k = parameterCount;
	while (k)
	{
		if (0 == strcmp(key,Commands[k-1]))
			return k;
		k--;
	}
	return 0;
}


void Game::Blit_Char(int xc,int yc,char c,int color)
{
	int offset,x,y;
	unsigned char work_char;
	unsigned char bit_mask = 0x80;

	// compute offset of character in video buffer
	offset = (yc << 8) + (yc << 6) + xc;

	for (y=0; y<8; y++)
	{
		// reset bit mask
		bit_mask = 0x80;

		// grab a row of character bits from the table    
		work_char = Game::char_gen_table[c][y];

		for (x=0; x<8; x++)
		{
			// test for transparent pixel
			// i.e. 0, if not transparent then draw

			if ((work_char & bit_mask))
				vbuff[offset+x] = color;
			else
				vbuff[offset+x] = TEXT_BKG_COLOR;

			// shift bit mask
			bit_mask = (bit_mask>>1);

		}

		// move to next line in video buffer
		offset += SCREEN_WIDTH;
	}
}

void Game::Blit_String(int x,int y,int color, char *string)
{
	int index;

	for (index=0; string[index]!=0; index++)
	{
		Blit_Char(x+(index<<3),y,string[index],color);
	}
}

void Game::BlitScreen(char *fname)
{
	FILE *fp;//, *fopen();
	int num_bytes,index;
	int count,isize,istep,iw,ih,x,y;
	unsigned char data;
	char fyle[12] = "";

	strcat(fyle,fname);
	strcat(fyle,".pcx");
	fp = fopen(fyle,"rb");
	for (index=0; index<128; index++)
		getc(fp);
	count=0;
	x = 0;
	y = 0;
	iw = 320;
	ih = 200;
	isize = iw * ih;
	while(count<=isize)
	{
		data = getc(fp);
		if ((data >= 192) && (data <= 255))
		{
			num_bytes = data - 192;
			data  = getc(fp);
			while(num_bytes-- > 0)
			{
				pp(x,y,(unsigned char)data);
				count++;
				y++;
				if (y == iw)
				{
					y = 0;
					x++;
				}
			}
		}
		else
		{
			pp(x,y,(unsigned char)data);
			count++;
			y++;
			if (y == iw)
			{
				y = 0;
				x++;
			}
		}
	}
	fclose(fp);
}

