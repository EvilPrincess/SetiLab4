#include "ServerNetwork.h"

extern HWND MainWnd;

ServerNetwork::ServerNetwork(void)
{
    WSADATA wsaData;

    ListenSocket = INVALID_SOCKET;
    ClientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        MessageBoxA(MainWnd, "Error", ("WSAStartup failed with error: " + std::to_string(iResult)).c_str(), MB_OK);
        PostQuitMessage(0);
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    if (iResult != 0) {
        MessageBoxA(MainWnd, "Error", ("getaddrinfo failed with error: " + std::to_string(iResult)).c_str(), MB_OK);
        WSACleanup();
        PostQuitMessage(0);
    }

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET) {
        MessageBoxA(MainWnd, "Error", ("socket failed with error: " + std::to_string(WSAGetLastError())).c_str(), MB_OK);
        freeaddrinfo(result);
        WSACleanup();
        PostQuitMessage(0);
    }

    u_long iMode = 1;
    iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);

    if (iResult == SOCKET_ERROR) {
        MessageBoxA(MainWnd, "Error", ("ioctlsocket failed with error: " + WSAGetLastError()).c_str(), MB_OK);
        closesocket(ListenSocket);
        WSACleanup();
        PostQuitMessage(0);
    }

    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

    if (iResult == SOCKET_ERROR) {
        MessageBoxA(MainWnd, "Error", ("bind failed with error: " + std::to_string(WSAGetLastError())).c_str(), MB_OK);
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        PostQuitMessage(0);
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);

    if (iResult == SOCKET_ERROR) {
        MessageBoxA(MainWnd, "Error", ("WSAStartup failed with error: " + std::to_string(WSAGetLastError())).c_str(), MB_OK);
        closesocket(ListenSocket);
        WSACleanup();
        PostQuitMessage(0);
    }
}

BOOL ServerNetwork::AcceptNewClient(UINT& id)
{
    ClientSocket = accept(ListenSocket, NULL, NULL);

    if (ClientSocket != INVALID_SOCKET)
    {
        char value = 1;
        setsockopt(ClientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

        sessions.insert(pair<unsigned int, SOCKET>(id, ClientSocket));

        return true;
    }

    return false;
}