#include "units.h"

#define WND_CLASS_NAME "My window class"
INT AN6_MouseWheel;

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
  INT i;

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

  for (i = 0; i < 1; i++)
    AN6_AnimUnitAdd(AN6_UnitCreateCOW());

  AN6_AnimUnitAdd(AN6_UnitCreateCONTROL());

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
}

INT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  PAINTSTRUCT ps;
  HDC hDC;
  static an6PRIM Pr;

  switch (Msg)
  {
  case WM_CREATE:
    AN6_AnimInit(hWnd);
    SetTimer(hWnd, 15, 10, NULL);
    return 0;
  
  case WM_SIZE: 
    AN6_RndResize(LOWORD(lParam), HIWORD(lParam));
    return 0;    
 
  case WM_TIMER:
    AN6_AnimRender();
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  
  case WM_KEYDOWN:
    if (wParam == VK_SPACE || wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    else if (wParam == 'F')
      FlipFullScreen(hWnd);
    return 0;

  case WM_MOUSEWHEEL:
    AN6_MouseWheel += (SHORT)HIWORD(wParam);
  return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    AN6_AnimCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_ERASEBKGND:
    return 1;

  case WM_DESTROY:
    AN6_AnimClose();
    KillTimer(hWnd, 15);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
