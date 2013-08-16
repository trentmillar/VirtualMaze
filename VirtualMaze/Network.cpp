#include "Network.h"

Network::Network(bool pisServer, char *pclientIP, int pPort)
{
	wVersionRequested = MAKEWORD(1,1);
	isServer = pisServer;
	clientIP = pclientIP;
}

Network::~Network(void)
{
}

int Network::StartCOM(void)
{
	Logger::Log("StartCOM(): Starting Network...\n");
	
	if (!InitSocket())
	{
		Logger::Log("StartCOM(): InitSocket() failed...\n");
	}

	if(isServer){
		StartServer();
		return TRUE;
	}

	if(clientIP){
		Logger::Log(clientIP);
		return StartClient(clientIP);
	}
	Logger::Log("Unknown error.\n");
	return FALSE;
}

void Network::StopCOM(void)
{
	WritePacket("Exit");
	Logger::Log("StopCOM(): Closing socket...\n");
	closesocket(theSocket);
	WSACleanup();
}

int Network::InitSocket()
{
	char ver[32];
	int serr;

	sprintf(ver,"Winsock version %04X\n",wVersionRequested);
	Logger::Log(ver);

	nRet = WSAStartup(wVersionRequested, &wsaData);
	if (wsaData.wVersion != wVersionRequested)
	{	
		Logger::Log("InitSocket(): Wrong winsock version.\n");
		return FALSE;
	}

	// get the name of the host 
	serr = gethostname(HostName, sizeof(HostName));
	if (serr == SOCKET_ERROR)
	{
		Logger::Log("gethostname()");
		Logger::Die("gethostname()",__FILE__,__LINE__);
		return FALSE;
	}

	return TRUE;
}

void Network::StartServer(void)
{
	int serr;
	u_long jjj = TRUE;

	//
	// Create a UDP/IP datagram socket
	//

	theSocket = socket(AF_INET,             // Address family
		SOCK_DGRAM,          // Socket type
		IPPROTO_UDP);        // Protocol

	if (theSocket == INVALID_SOCKET)
	{
		Logger::Log("socket()");
		Logger::Die("socket()", __FILE__, __LINE__);
		//               return FALSE;
	}

	//make socket non-blocking
	serr = ioctlsocket(theSocket,FIONBIO, (u_long FAR *)&jjj);
	if (serr == SOCKET_ERROR)
	{
		Logger::Log("non-blocking socket()");
		Logger::Die("non-blocking socket()", __FILE__, __LINE__);
		//               return FALSE;
	}

	//
	// Fill in the address structure for the server
	//

	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = INADDR_ANY;                           
	saServer.sin_port = htons(nPort);

	//bind the name to the socket

	serr = bind(theSocket,
		(LPSOCKADDR)&saServer,
		sizeof(struct sockaddr));
	if (serr == SOCKET_ERROR)
	{
		Logger::Log("bind()");
		Logger::Die("bind()", __FILE__, __LINE__);
	}
	//waiting = TRUE;
}

int Network::StartClient(char *server)
{
	int serr;
	u_long jjj = TRUE;
	char msg[64];

	//
	// Find the server
	//

	lpHostEntry = gethostbyname(server);
	if (lpHostEntry == NULL)
	{
		Logger::Log("StartClient(): gethostbyname()\n");
		return FALSE;
	}

	//
	// Create a UDP/IP datagram socket
	//

	theSocket = socket(AF_INET,             // Address family
		SOCK_DGRAM,          // Socket type
		IPPROTO_UDP);        // Protocol

	if (theSocket == INVALID_SOCKET)
	{
		Logger::Log("socket()");
		Logger::Die("socket()", __FILE__, __LINE__);
		return FALSE;
	}

	//make socket non-blocking
	serr = ioctlsocket(theSocket,FIONBIO, (u_long FAR *)&jjj);
	if (serr == SOCKET_ERROR)
	{
		Logger::Log("non-blocking socket()");
		Logger::Die("non-blocking socket()", __FILE__, __LINE__);
		return FALSE;
	}

	//
	// Fill in the address structure for the server
	//

	saServer.sin_family = AF_INET;
	saServer.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list);                           
	saServer.sin_port = htons(nPort);

	sprintf(msg,"Join%s",HostName);
	wait_for_ack(msg);
	return TRUE;
}

void Network::WritePacket(char *pdata)
{
	//
	// Send data to the server
	//
	int nRet;


	strcpy(BufferSize,pdata);
	nRet = sendto(theSocket,                // Socket
		BufferSize,                    // Data buffer
		strlen(BufferSize),            // Length of data
		0,                        // Flags
		(LPSOCKADDR)&saServer,    // Server address
		sizeof(struct sockaddr)); // Length of address

	if (nRet == SOCKET_ERROR)
	{
		Logger::Log("sendto()");
		Logger::Die("sendto()", __FILE__, __LINE__);
		closesocket(theSocket);
	}
}

void Network::wait_for_ack(char *pdata)
{
	int k, ps, acked = FALSE;

	while (!acked)
	{
		WritePacket(pdata);
		Logger::Log("joining");
		k = 0;
		while ((k < 100) && !acked)
		{
			ps = ReadPacket();
			Logger::Log(BufferSize);
			if ('A' == BufferSize[0])
				acked = TRUE;
			k++;
		}
	}
}

int Network::ReadPacket(void)
{
	int nFromLen;

	//
	// Wait for the reply
	//
	memset(BufferSize, 0, sizeof(BufferSize));

	nFromLen = sizeof(struct sockaddr);
	recvfrom(theSocket,             // Socket
		BufferSize,                 // Receive buffer
		sizeof(BufferSize),         // Length of receive buffer
		0,                     // Flags
		(LPSOCKADDR)&saServer, // Buffer to receive sender's address
		&nFromLen);            // Length of address buffer

	if (nRet == SOCKET_ERROR)
	{
		Logger::Log("recvfrom()");
		Logger::Die("recvfrom()", __FILE__, __LINE__);
		closesocket(theSocket);
		return FALSE;
	}
	return TRUE;
}
