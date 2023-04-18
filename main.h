#pragma once

#include "base.h"

#define MAIN_WC L"MainWndClass"
#define BUFFERSIZE 16384
#define DEFAULT_PORT 1111
#define DEFAULT_IP "127.0.0.1"


//		THREADS
//
#define SERVER_T 1
#define RECEIVE_T 2
#define ACCEPT_T 3

//		COMMANDS
//
#define OnStartPressed 0
#define OnStopPressed 1

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
LRESULT CommandHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OnMainWindowCreated(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
inline void DrawServer(HWND hWnd);
inline void OnResize(HWND hWnd);
inline void ResizeWindows(HWND hWnd);
void CreateWidgets(HWND hWnd);
void MB(string _Msg, BOOL _IsWarning = FALSE);
void DM(string _Msg, string _End = "\r\n");
string strrmv(string str, char ch);
DWORD WINAPI ServerHandler(LPVOID lpParam);
void Init();
void Stop();
DWORD WINAPI ReceiveProc(LPVOID lpParam);
DWORD WINAPI AcceptProc(LPVOID lpParam);

struct CLIENT
{
	string name;
	SOCKET sock;
	BOOL recv;
};