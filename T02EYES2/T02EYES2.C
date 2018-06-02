#include <stdio.h>
#include <windows.h>
#include <math.h>
#define WND_CLASS_NAME "My window class"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

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
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }

  hWnd = CreateWindow(WND_CLASS_NAME, "EYES",
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

VOID DrawEye(HDC hDC, INT x, INT y, INT r, INT r1, INT Mx, INT My)
{
  INT alp = arctan(abs((x - Mx) / (y - My)));

  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));

  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));

  Ellipse(hDC, x - r, y - r, x + r, y + r);

  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(0, 0, 0));

  Ellipse(hDC, x + r, y + r, 2 * r + x, 2 * r + y);
}
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  POINT pt;
  RECT rc;
  HDC hDC;

  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 15, 30, NULL);
    return 0;

  case WM_TIMER:
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    hDC = GetDC(hWnd);
    GetClientRect(hWnd, &rc);

    DrawEye(hDC, 200, 200, 100, 50, pt.x, pt.y);

    

    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}





