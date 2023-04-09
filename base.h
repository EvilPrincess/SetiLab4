#pragma once

#include <winsock2.h>
#include <windows.h>
#include <WS2tcpip.h>
#include <string>
#include <map>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "6881"

using namespace std;