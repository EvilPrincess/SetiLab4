#include "INetwork.h"

extern HWND MainWnd;

int INetwork::sendMessage(SOCKET curSocket, char* message, int messageSize)
{
	return send(curSocket, message, messageSize, 0);
}

int INetwork::receiveMessage(SOCKET curSocket, char* buffer, int bufSize)
{
	return recv(curSocket, buffer, bufSize, 0);
}

INetwork::INetwork()
{
	client_id = 0;

}

void INetwork::Update()
{
    if (network->AcceptNewClient(client_id))
    {
        MessageBoxA(MainWnd, "Саксесс!", ("Client " + to_string(client_id) + " has been connected!").c_str(), MB_OK);
    }
}