#pragma once

#include <winsock2.h>
#include <stdio.h>
#include <string.h>
#include "Logger.h"

//extern int logged, waiting;
//extern char cmds[10][32];

class Network
{
public:
	Network(bool pIsServer, char *pClientIP, int pPort);
	~Network(void);

	int StartCOM(void);
	void StopCOM(void);
	int InitSocket(void);
	void StartServer(void);
	int StartClient(char *server);
	void WritePacket(char *pdata);
	int ReadPacket(void);
	
	char HostName[256];
	char BufferSize[256];

	short nPort;
	WORD wVersionRequested;
	WSADATA wsaData;
	int nRet;
	LPHOSTENT lpHostEntry;
	SOCKET	theSocket;
	SOCKADDR_IN saServer;

	char statstr[64];
	int statmsg,statime;

	char transmsg[64];
	int intrans, transptr;
	int nofloor;

private:
	char *clientIP;
	bool isServer;

	void wait_for_ack(char *pdata);
};