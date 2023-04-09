#pragma once

#include "base.h"
#include "INetwork.h"

class ServerNetwork
{
private:
public:
	ServerNetwork();
	BOOL AcceptNewClient(UINT& id);

	SOCKET ListenSocket;
	SOCKET ClientSocket;
	INT iResult;
	map<UINT, SOCKET> sessions;
};

typedef ServerNetwork SN;