#include "main.h"

using namespace std;

//
//		GLOBAL VARIABLES
//
HWND MainWnd = { };
HANDLE ServerThread = { };
BOOL running = TRUE;

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
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}
LRESULT CommandHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}
void OnMainWindowCreated(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Init();
}
void MB(string _Msg, string _End, UINT _Style)
{
	MessageBoxA(NULL, (_Msg + _End).c_str(), "Да", _Style);
}
DWORD WINAPI ServerHandler(LPVOID lpParam)
{
	SOCKADDR_IN addr;
	int size = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr(DEFAULT_IP);
	addr.sin_port = htons(DEFAULT_PORT);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;

	MB("Waiting for new connections...");

	return 1;
}
void Init()
{
	WSADATA WSAdata;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &WSAdata) != 0)
	{
		MB("no wsa cry :(");
		PostQuitMessage(0);
	}
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
	running = FALSE;
	WSACleanup();
}