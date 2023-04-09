#pragma once

#include <WS2tcpip.h>
#include "base.h"
#include "INetwork.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6881"
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

class ClientNetwork
{
private:
	int iResult;
	SOCKET ConnectSocket;
public:
	ClientNetwork();
};

typedef ClientNetwork CN;

