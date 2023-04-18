#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "WS2_32.lib")
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <vector>
#include <tchar.h>

#define DEFAULT_BUFLEN 16384
#define DEFAULT_PORT "6881"

using namespace std;