#pragma once

#include "base.h"
#include "INetwork.h"

class ServerNetwork
{
private:
public:
	ServerNetwork();
	~ServerNetwork();
	SOCKET ListenSocket;
	SOCKET ClientSocket;
	INT iResult;
	map<UINT, SOCKET> sessions;
};

typedef ServerNetwork SN;