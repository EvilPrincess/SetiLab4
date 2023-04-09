#include "main.h"

using namespace std;

//
//		GLOBAL VARIABLES
//
HWND MainWnd = { };



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	WNDCLASS MainWndClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst,
		LoadIcon(NULL, IDI_QUESTION), MAIN_WC, MainWndProc);

	if (!RegisterClassW(&MainWndClass)) { return -1; }

	MSG MainWndMessage = { };

	MainWnd = CreateWindow(MAIN_WC, L"Так называемый почтовый сервер", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, 100, 100, 1200, 700, NULL, NULL, NULL, NULL);
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