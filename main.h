#pragma once

#include "base.h"

#define MAIN_WC L"MainWndClass"
#define CLIENT_T 0
#define BUFFERSIZE 16384
#define DEFAULT_PORT 1111
#define DEFAULT_IP "127.0.01"

//		COMMANDS
//
#define OnEnterPressed 0
#define OnExitPressed 1
#define OnSendPressed 2

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
LRESULT CommandHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OnMainWindowCreated(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void MB(string _Msg, BOOL _IsWarning = FALSE);
void DM(string _Msg, string _End = "\r\n");
DWORD WINAPI ClientHandler(LPVOID lpParam);
void Enter();
void Exit();
inline void DrawClient(HWND hWnd);
inline void OnResize(HWND hWnd);
inline void ResizeWindows(HWND hWnd);
void CreateWidgets(HWND hWnd);