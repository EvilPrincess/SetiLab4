#include "main.h"

//
//		GLOBAL VARIABLES
//
HWND MainWnd = { };
HWND EditBox = { };
HWND EnterBtn = { };
HWND ExitBtn = { };
HWND SendBtn = { };
HWND MsgBox = { };
HWND IPENTERBOX = NULL;
HANDLE ClientThread = { };
BOOL running = FALSE;
CHAR BUFFER[BUFFERSIZE] = { };
SOCKET client = { };
char ip[16] = { };
vector<string> msgStack{ };
char name[256] = "ДядяПётр";



//
//		ВИНАПИШНЫЕ ФУНКЦИИ
//
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	WNDCLASS MainWndClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst,
		LoadIcon(NULL, IDI_QUESTION), MAIN_WC, MainWndProc);
	WNDCLASS EnterIPWndClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst,
		LoadIcon(NULL, IDI_QUESTION), IPENTER_WC, EnterIPWndProc);

	if (!RegisterClassW(&MainWndClass)) { return -1; }
	if (!RegisterClassW(&EnterIPWndClass)) { return -1; }

	MSG MainWndMessage = { };

	MainWnd = CreateWindow(MAIN_WC, L"Так называемый почтовый клиент", WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 1200, 700, NULL, NULL, NULL, NULL);
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
		Exit();
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
		DrawClient(hWnd);
		break;
	}
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}
LRESULT CALLBACK EnterIPWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
		{
			IPENTERBOX = CreateWindowA("edit", ip, WS_CHILD | WS_BORDER | WS_VISIBLE, 10, 10, 160, 20, hWnd, (HMENU)IP_WC, NULL, NULL);
			CreateWindowA("button", "Подтвердить", WS_CHILD | WS_BORDER | WS_VISIBLE, 10, 40, 160, 20, hWnd, (HMENU)OnIPApplyPressed, NULL, NULL);
			break;
		}
		case WM_COMMAND:
		{
			switch (wParam)
			{
				case OnIPApplyPressed:
				{
					GetWindowTextA(IPENTERBOX, ip, 16);
					IPENTERBOX = NULL;
					EnableWindow(EnterBtn, 1);
					DestroyWindow(hWnd);
					break;
				}
			}
			break;
		}
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}
inline void OnResize(HWND hWnd)
{
	ResizeWindows(hWnd);
	DrawClient(hWnd);
}
inline void ResizeWindows(HWND hWnd)
{
	RECT r;
	GetClientRect(hWnd, &r);
	MoveWindow(EditBox, 11, 11, r.right - 32, r.bottom - 112, TRUE);
	/*MoveWindow(EnterBtn, 10, r.bottom - 90, r.right / 2 - 20, 80, TRUE);
	MoveWindow(ExitBtn, r.right / 2, r.bottom - 90, r.right / 2 - 20, 80, TRUE);*/
}
inline void DrawClient(HWND hWnd)
{
	HDC hDC;
	RECT r;

	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);

	hDC = GetDC(hWnd);
	GetClientRect(hWnd, &r);

	Rectangle(hDC, 10, 10, r.right - 20, r.bottom - 100);
	Rectangle(hDC, 10, r.bottom - 90, r.right - 200, r.bottom - 10);

	ReleaseDC(hWnd, hDC);
}
LRESULT CommandHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
		case EN_SETFOCUS:
		{
			if (LOWORD(wParam) == MSG_WC)
			{
				char buf[256];
				GetWindowTextA(MsgBox, buf, 256);
				if (string(buf) == "Введите сообщение...")
					SetWindowTextA(MsgBox, "");
			}
			break;
		}
		case EN_KILLFOCUS:
		{
			if (LOWORD(wParam) == MSG_WC)
			{
				char buf[256];
				GetWindowTextA(MsgBox, buf, 256);
				if (string(buf) == "")
					SetWindowTextA(MsgBox, "Введите сообщение...");
			}
			break;
		}
	}
	switch (wParam)
	{
	case OnEnterPressed:
	{
		Enter();
		break;
	}
	case OnExitPressed:
	{
		Exit();
		break;
	}
	case OnSendPressed:
	{
		Send();
		break;
	}
	case OnOpenIPWndPressed:
	{
		if (IPENTERBOX == NULL)
			CreateWindow(IPENTER_WC,  L"Ввод адреса назначения", WS_OVERLAPPED | WS_VISIBLE, 200, 200, 200, 110, hWnd, NULL, NULL, NULL);
		break;
	}
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}
void OnMainWindowCreated(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DrawClient(hWnd);
	CreateMenus(hWnd);
	CreateWidgets(hWnd);
}
void CreateWidgets(HWND hWnd)
{
	RECT r;
	GetClientRect(hWnd, &r);
	EditBox = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY |
		ES_AUTOVSCROLL | WS_VSCROLL, 11, 11, r.right - 22, r.bottom - 112, hWnd, NULL, NULL, NULL);
	MsgBox = CreateWindowA("EDIT", "Введите сообщение...", WS_CHILD | WS_VISIBLE | ES_MULTILINE,
		11, r.bottom - 89, r.right - 120 - 100 + 10 - 2, 80 - 2, hWnd, (HMENU)MSG_WC, NULL, NULL);
	EnterBtn = CreateWindowA("button", "Войти", WS_CHILD | WS_VISIBLE | WS_DISABLED,
		r.right - 100, r.bottom - 90, 80, 35, hWnd, (HMENU)OnEnterPressed, NULL, NULL);
	ExitBtn = CreateWindowA("button", "Выйти", WS_CHILD | WS_VISIBLE | WS_DISABLED,
		r.right - 100, r.bottom - 90 + 40, 80, 35, hWnd, (HMENU)OnExitPressed, NULL, NULL);
	SendBtn = CreateWindowA("button", "Отправить", WS_CHILD | WS_VISIBLE,
		r.right - 200 + 10, r.bottom - 90, 80, 80, hWnd, (HMENU)OnSendPressed, NULL, NULL);
}
void CreateMenus(HWND hWnd)
{
	HMENU RootMenu = CreateMenu();

	AppendMenuA(RootMenu, MF_STRING, OnOpenIPWndPressed, "Задать адрес назначения");

	SetMenu(hWnd, RootMenu);
}
//
//		ВСПОМОГАТЕЛЬНЫЕ
//
void MB(string _Msg, BOOL _IsWarning)
{
	MessageBoxA(NULL, _Msg.c_str(), "Да", _IsWarning ? MB_OK | MB_ICONERROR : MB_OK);
}
void DM(string _Msg, string _End)
{
	GetWindowTextA(EditBox, BUFFER, BUFFERSIZE);
	SetWindowTextA(EditBox, (string(BUFFER) + _Msg + _End).c_str());
}
//
//		ЛАБА
//
DWORD WINAPI ClientHandler(LPVOID lpParam)
{
	SOCKADDR_IN addr;
	int size = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(DEFAULT_PORT);
	addr.sin_family = AF_INET;

	DM("$ Подключение к серверу...");
	if ((client = socket(AF_INET, SOCK_STREAM, NULL)) == SOCKET_ERROR)
	{
		MB("Ошибка функции socket: " + to_string(WSAGetLastError()), TRUE);
		DM("$ Ошибка функции socket.");
		Exit(0);
		return 1;
	}
	if ((connect(client, (SOCKADDR*)&addr, sizeof(addr))) == SOCKET_ERROR)
	{
		MB("Ошибка функции connect: " + to_string(WSAGetLastError()), TRUE);
		DM("$ Ошибка подключения к серверу.");
		Exit(0);
		return 1;
	}
	send(client, name, 256, NULL);
	recv(client, BUFFER, 256, NULL);
	if (string(BUFFER) == "Клиент с таким именем уже подключен к серверу!")
	{
		DM("$ Клиент с таким именем уже подключен к серверу!");
		Exit();
	}

	DM("$ Успешное подключение к серверу!");


	//for (string msg : msgStack)
	//{
	//	send(client, msg.c_str(), sizeof(msg), NULL);
	//}
	//msgStack.clear();

	return 1;
}
void Enter()
{
	if (running) return;
	EnableWindow(EnterBtn, 0);
	EnableWindow(ExitBtn, 1);
	WSADATA WSAdata;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &WSAdata) != 0)
	{
		MB("no wsa cry :(");
		PostQuitMessage(0);
	}
	running = TRUE;
	ClientThread = CreateThread(
		NULL,
		0,
		ClientHandler,
		CLIENT_T,
		0,
		NULL);
}
void Exit(BOOL wasConnected)
{
	if (!running) return;
	EnableWindow(EnterBtn, 1);
	EnableWindow(ExitBtn, 0);
	running = FALSE;
	char buf[] = "$ disconnect";
	send(client, buf, sizeof(buf), NULL);
	closesocket(client);
	if (WSACleanup() == SOCKET_ERROR)
	{
		MB("Ошибка функции WSACleanup", MB_OK | MB_ICONERROR);
	}
	else
	{
		if (wasConnected) DM("$ Соединение разорвано.");
	}
}
void Send()
{
	char buffer[256];
	GetWindowTextA(MsgBox, buffer, 256);
	if (string(buffer) == "$ disconnect") Exit();
	else 
		if (send(client, buffer, sizeof(buffer), NULL) == SOCKET_ERROR)
		{
			msgStack.push_back(buffer);
		}
	DM("> " + string(buffer));
	SetWindowTextA(MsgBox, "Введите сообщение...");
}