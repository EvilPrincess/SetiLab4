#pragma once

#include "base.h"
#include "ClientNetwork.h"
#include "ServerNetwork.h"

class INetwork
{
private:
	static UINT client_id;
	CN* client;
	SN* network;
public:
	INetwork();
	void Update();
	static int sendMessage(SOCKET curSocket, char* message, int messageSize);
	static int receiveMessage(SOCKET curSocket, char* buffer, int bufSize);
};

typedef INetwork Inet;