#pragma once

#include "base.h"
#include "INetwork.h"

class ClientNetwork
{
private:
public:
	INT iResult;
	SOCKET ConnectSocket;
	ClientNetwork();
};

typedef ClientNetwork CN;

