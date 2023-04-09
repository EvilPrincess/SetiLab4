#pragma once

#include "base.h"

class INetwork
{
	static int sendMessage(SOCKET curSocket, char* message, int messageSize);
	static int receiveMessage(SOCKET curSocket, char* buffer, int bufSize);
};

typedef INetwork Inet;