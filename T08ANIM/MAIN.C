#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <time.h>
#include "anim\rnd\rnd.h"
#define WND_CLASS_NAME "My window class"


INT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRC;

  if (IsFullScreen)
  {
    IsFullScreen = FALSE;

    SetWindowPos(hWnd, HWND_TOP, SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top, SWP_NOOWNERZORDER);
  }
  else
  {
    RECT rc;
    HMONITOR hMon;
    MONITORINFOEX moninfo;

    IsFullScreen = TRUE;

    GetWindowRect(hWnd, &SaveRC);

    hMon = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
    moninfo.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(hMon, (MONITORINFO *)&moninfo);

    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);

    SetWindowPos(hWnd, HWND_TOP, rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top, SWP_NOOWNERZORDER);
  }
}

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd)
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0; 
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindow(WND_CLASS_NAME, "ANIM",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT,
    CW_USEDEFAULT, CW_USEDEFAULT,
    NULL,
    NULL,
    hInstance,
    NULL);
  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
}
typedef struct
{
  DOUBLE x, y, z;
} VECT;

INT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  static INT w, h;
  static HDC hMemDC;
  static HBITMAP hBm;
  static HFONT hFnt;
  POINT pt;
  PAINTSTRUCT ps;
  HDC hDC;

  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);

  switch (Msg)
  {
  case WM_CREATE:
    AN6_RndInit();

    return 0;
  
  case WM_SIZE:
    AN6_RndResize(LOWORD(lParam), HIWORD(lParam));

    return 0;
 
  case WM_TIMER:
    AN6_RndStart();
    AN6_RndEnd();

    InvalidateRect(hWnd, NULL, TRUE);

    return 0;
  
  case WM_KEYDOWN:
	  if (wParam == VK_SPACE || wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    else if (wParam == 'F')
      FlipFullScreen(hWnd);
  
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);

    return 0;

  case WM_ERASEBKGND:
    return 1;

  case WM_DESTROY:
    DeleteObject(hBm);
    DeleteDC(hMemDC);
    KillTimer(hWnd, 15);
    PostQuitMessage(0);

    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}