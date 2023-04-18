#include "main.h"

//
//		GLOBAL VARIABLES
//
HWND MainWnd = { };
HWND EditBox = { };
HWND StartBtn = { };
HWND StopBtn = { };
HANDLE ServerThread = { };
BOOL running = FALSE;
BOOL receiving = FALSE;
CHAR BUFFER[BUFFERSIZE] = { };
SOCKET server, client;
SOCKADDR_IN addr;
UINT client_id = 0;

vector<CLIENT> clients;

int addr_size = sizeof(addr);


//
//		ВИНАПИШНЫЕ ФУНКЦИИ
//
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	WNDCLASS MainWndClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst,
		LoadIcon(NULL, IDI_QUESTION), MAIN_WC, MainWndProc);

	if (!RegisterClassW(&MainWndClass)) { return -1; }

	MSG MainWndMessage = { };

	MainWnd = CreateWindow(MAIN_WC, L"Так называемый почтовый сервер", WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 1200, 700, NULL, NULL, NULL, NULL);
	while (GetMessage(&MainWndMessage, NULL, NULL, NULL)) {
		TranslateMessage(&MainWndMessage);
		DispatchMessage(&MainWndMessage);
	}
}
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure) {
	WNDCLASS NWC = { 0 };

	NWC.hIcon = Icon;
	NWC.hCursor = Cursor;
	NWC.hInstance = hInst;
	NWC.lpszClassName = Name;
	NWC.hbrBackground = BGColor;
	NWC.lpfnWndProc = Procedure;

	return NWC;
}
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
		{
			OnMainWindowCreated(hWnd, uMsg, wParam, lParam);
			break;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			break;
		}
		case WM_COMMAND:
		{
			return CommandHandler(hWnd, uMsg, wParam, lParam);
			break;
		}
		case WM_SIZE:
		{
			OnResize(hWnd);
			return 0;
		}
		case WM_MOVE:
		{
			OnResize(hWnd);
			break;
		}
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}
inline void OnResize(HWND hWnd)
{
	ResizeWindows(hWnd);
	DrawServer(hWnd);
}
inline void ResizeWindows(HWND hWnd)
{
	RECT r;
	GetClientRect(hWnd, &r);
	MoveWindow(EditBox, 11, 11, r.right - 32, r.bottom - 112, TRUE);
	MoveWindow(StartBtn, 10, r.bottom - 90, r.right / 2 - 20, 80, TRUE);
	MoveWindow(StopBtn, r.right / 2, r.bottom - 90, r.right / 2 - 20, 80, TRUE);
}
inline void DrawServer(HWND hWnd)
{
	HDC hDC;
	RECT r;

	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);

	hDC = GetDC(hWnd);
	GetClientRect(hWnd, &r);

	Rectangle(hDC, 10, 10, r.right-20, r.bottom - 100);

	ReleaseDC(hWnd, hDC);
}
LRESULT CommandHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case OnStartPressed:
		{
			Init();
			break;
		}
		case OnStopPressed:
		{
			Stop();
			break;
		}
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}
void OnMainWindowCreated(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DrawServer(hWnd);
	CreateWidgets(hWnd);
	
}
void CreateWidgets(HWND hWnd)
{
	RECT r; 
	GetClientRect(hWnd, &r);
	EditBox = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | 
		ES_AUTOVSCROLL | WS_VSCROLL, 11, 11, r.right - 22, r.bottom - 112, hWnd, NULL, NULL, NULL);
	StartBtn = CreateWindowA("BUTTON", "Включить", WS_CHILD | WS_VISIBLE | SS_CENTER,
		10, r.bottom - 90, r.right / 2 - 20, 80, hWnd, (HMENU)OnStartPressed, NULL, NULL);
	StopBtn = CreateWindowA("BUTTON", "Отключить", WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_CENTER,
		r.right / 2, r.bottom - 90, r.right / 2 - 20, 80, hWnd, (HMENU)OnStopPressed, NULL, NULL);
}
//
//		ВСПОМОГАТЕЛЬНЫЕ
//
void MB(string _Msg, BOOL _IsWarning)
{
	MessageBoxA(NULL, _Msg.c_str(), "Да", _IsWarning? MB_OK | MB_ICONERROR : MB_OK);
}
void DM(string _Msg, string _End)
{
	GetWindowTextA(EditBox, BUFFER, BUFFERSIZE);
	SetWindowTextA(EditBox, (string(BUFFER) + _Msg + _End).c_str());
}
//
//		ЛАБА
//
void Init()
{
	if (running) return;
	EnableWindow(StartBtn, 0);
	EnableWindow(StopBtn, 1);
	WSADATA WSAdata;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &WSAdata) != 0)
	{
		MB("no wsa cry :(");
		PostQuitMessage(0);
	}
	running = TRUE;
	ServerThread = CreateThread(
		NULL,
		0,
		ServerHandler,
		(LPVOID)SERVER_T,
		0,
		NULL);
}
DWORD WINAPI ServerHandler(LPVOID lpParam)
{
	addr.sin_addr.s_addr = inet_addr(DEFAULT_IP);
	addr.sin_port = htons(DEFAULT_PORT);
	addr.sin_family = AF_INET;

	if ((server = socket(AF_INET, SOCK_STREAM, NULL)) == SOCKET_ERROR)
	{
		MB("Ошибка функции socket: " + to_string(WSAGetLastError()), TRUE);
		return 1;
	}
	if ((bind(server, (SOCKADDR*)&addr, sizeof(addr))) == SOCKET_ERROR)
	{
		MB("Ошибка функции bind: " + to_string(WSAGetLastError()), TRUE);
		return 1;
	}
	DM("$ Успешное создание сервера!");

	if ((listen(server, SOMAXCONN)) == SOCKET_ERROR)
	{
		MB("Ошибка функции listen: " + to_string(WSAGetLastError()), TRUE);
		return 1;
	}
	
	DM("$ Ожидание клиентов...");
	CreateThread(
		NULL,
		0,
		AcceptProc,
		(LPVOID)ACCEPT_T,
		0,
		NULL);

	return 1;
}
void Stop()
{
	if (!running) return;
	EnableWindow(StartBtn, 1);
	EnableWindow(StopBtn, 0);
	running = FALSE;
	receiving = FALSE;
	closesocket(client);
	closesocket(server);
	if (WSACleanup() == SOCKET_ERROR)
	{
		MB("Ошибка функции WSACleanup", TRUE);
	}
	else
	{
		DM("$ Сервер отключен.");
	}
}

DWORD WINAPI ReceiveProc(LPVOID lpParam)
{
	char buffer[1024];
	while(receiving)
	{
		recv(clients[(UINT)lpParam].sock, buffer, sizeof(buffer), NULL);
		if (string(buffer) == "$ disconnect")
		{
			DM("$ Клиент " + clients[(UINT)lpParam].name + " отключился. Очень жаль...");
			return 1;
		}
		DM("Клиент " + clients[(UINT)lpParam].name + " > " + string(buffer));
	}
}
DWORD WINAPI AcceptProc(LPVOID lpParam)
{
	while (running)
	{
		if ((client = accept(server, (SOCKADDR*)&addr, &addr_size)) == SOCKET_ERROR)
		{
			MB("Ошибка функции accept: " + to_string(WSAGetLastError()), TRUE);
			running = FALSE;
			return 1;
		}

		DM("$ Новый клиент присоединился!");
		char name[256];
		recv(client, name, 256, NULL);
		int clid = -1;
		for (int i = 0; i < clients.size(); i++)
		{
			if (clients[i].name == string(name))
			{
				clid = i;
			}
		}
		if (clid == -1)
		{
			clients.push_back(CLIENT{ string(name), client, TRUE });
			clid = clients.size() - 1;
		}

		CreateThread(
			NULL,
			0,
			ReceiveProc,
			(LPVOID)clid,
			0,
			NULL);
	}

	return 1;
}