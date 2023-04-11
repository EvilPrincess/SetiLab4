#pragma once

#include "base.h"

#define MAIN_WC L"MainWndClass"
#define SERVER_T 0
#define DEFAULT_PORT 1111
#define DEFAULT_IP "127.0.01"

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure);
LRESULT CommandHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void OnMainWindowCreated(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void MB(string _Msg, string _End = "\r\n", UINT _Style = MB_OK | MB_ICONERROR);
DWORD WINAPI ServerHandler(LPVOID lpParam);
void Init();
void Stop();
