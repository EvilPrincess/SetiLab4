#include "ClientNetwork.h"


//
//		GLOBAL VARIABLES
//
extern HWND MainWnd;



ClientNetwork::ClientNetwork()
{
	WSADATA wsaData;
	ConnectSocket = INVALID_SOCKET;

	struct addrinfo* result = NULL, * ptr = NULL, hints;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0)
	{
		MessageBoxA(MainWnd, "Error", ("WSAStartup failed with error: " + std::to_string(iResult)).c_str(), MB_OK);
		PostQuitMessage(0);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);

	if (iResult != 0)
	{
		MessageBoxA(MainWnd, "Error", ("getaddrinfo failed with error: " + std::to_string(iResult)).c_str(), MB_OK);
		PostQuitMessage(0);
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) 
		{
			MessageBoxA(MainWnd, "Error", ("socket failed with error: " + std::to_string(WSAGetLastError())).c_str(), MB_OK);
			WSACleanup();
			PostQuitMessage(0);
		}

		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

		if (iResult == SOCKET_ERROR)
		{
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			MessageBoxA(MainWnd, "Error", "The server is down... did not correct", MB_OK);
		}
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET)
	{
		MessageBoxA(MainWnd, "Error", "Unable to connect to server!", MB_OK);
		WSACleanup();
		PostQuitMessage(0);
	}

	ULONG iMode = 1;

	iResult = ioctlsocket(ConnectSocket, FIONBIO, &iMode);
	if (iResult == SOCKET_ERROR)
	{
		MessageBoxA(MainWnd, "Error", ("ioctlsocket failed with error: " + std::to_string(WSAGetLastError())).c_str(), MB_OK);
		closesocket(ConnectSocket);
		WSACleanup();
		PostQuitMessage(0);
	}

	char value = 1;
	setsockopt(ConnectSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));
}