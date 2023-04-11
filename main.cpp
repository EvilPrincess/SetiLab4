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
CHAR BUFFER[BUFFERSIZE] = { };


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	WNDCLASS MainWndClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst,
		LoadIcon(NULL, IDI_QUESTION), MAIN_WC, MainWndProc);

	if (!RegisterClassW(&MainWndClass)) { return -1; }

	MSG MainWndMessage = { };

	MainWnd = CreateWindow(MAIN_WC, L"Так называемый почтовый сервер", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 1200, 700, NULL, NULL, NULL, NULL);
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
	StopBtn = CreateWindowA("BUTTON", "Отключить", WS_CHILD | WS_VISIBLE | SS_CENTER,
		r.right / 2, r.bottom - 90, r.right / 2 - 20, 80, hWnd, (HMENU)OnStopPressed, NULL, NULL);
}
void MB(string _Msg, UINT _Style)
{
	MessageBoxA(NULL, _Msg.c_str(), "Да", _Style);
}
void DM(string _Msg, string _End)
{
	GetWindowTextA(EditBox, BUFFER, BUFFERSIZE);
	SetWindowTextA(EditBox, (string(BUFFER) + _Msg + _End).c_str());
}
DWORD WINAPI ServerHandler(LPVOID lpParam)
{
	SOCKADDR_IN addr;
	int size = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr(DEFAULT_IP);
	addr.sin_port = htons(DEFAULT_PORT);
	addr.sin_family = AF_INET;

	SOCKET sListen;
	if ((sListen = socket(AF_INET, SOCK_STREAM, NULL)) == SOCKET_ERROR)
	{
		MB("ащипка создания сокета", MB_OK | MB_ICONERROR);
		return 1;
	}
	if ((bind(sListen, (SOCKADDR*)&addr, sizeof(addr))) == SOCKET_ERROR)
	{
		MB(to_string(WSAGetLastError()).c_str(), MB_OK | MB_ICONERROR);
		return 1;
	}

	DM("Успешное создание сервера!");

	if ((listen(sListen, SOMAXCONN)) == SOCKET_ERROR)
	{
		MB("ащипка прослушивания", MB_OK | MB_ICONERROR);
		return 1;
	}

	SOCKET newConnection;

	DM("Ожидание клиентов...");

	return 1;
}
void Init()
{
	if (running) return;
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
		SERVER_T,
		0,
		NULL);
}
void Stop()
{
	if (!running) return;
	running = FALSE;
	if (WSACleanup() == SOCKET_ERROR)
	{
		MB("Ошибка функции WSACleanup", MB_OK | MB_ICONERROR);
	}
	else
	{
		DM("Сервер отключен.");
	}
}