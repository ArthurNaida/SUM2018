#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <time.h>
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
    MessageBox(NULL, "Error", "ERROR", MB_OK);
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
  DOUBLE dx = ((r - r1) * (Mx - x)) / (sqrt((Mx - x) * (Mx - x) + (My - y) * (My - y)));
  DOUBLE dy = ((r - r1) * (My - y)) / (sqrt((Mx - x) * (Mx - x) + (My - y) * (My - y)));

  SelectObject(hDC, GetStockObject(DC_PEN));
  SelectObject(hDC, GetStockObject(DC_BRUSH));

  SetDCPenColor(hDC, RGB(0, 0, 0));
  SetDCBrushColor(hDC, RGB(255, 255, 255));

  Ellipse(hDC, x - r, y - r, x + r, y + r);
  
  if (sqrt((Mx - x) * (Mx - x) + (My - y) * (My - y)) < (r - r1))
  {
    SetDCPenColor(hDC, RGB(0, 0, 0));
    SetDCBrushColor(hDC, RGB(0, 0, 0));

    Ellipse(hDC, Mx - r1, My - r1, Mx + r1, My + r1);
  }
  else
  {
    SetDCPenColor(hDC, RGB(0, 0, 0));
    SetDCBrushColor(hDC, RGB(0, 0, 0));

    Ellipse(hDC, dx + x - r1, dy + y - r1, dx + x + r1, dy + y + r1);
  }
}
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  POINT pt;
  PAINTSTRUCT ps;
  static INT w, h;
  HDC hDC;
  static HDC hMemDC;
  static HBITMAP hBm;

  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
	hMemDC = CreateCompatibleDC(hDC);
	ReleaseDC(hWnd, hDC);
	SetTimer(hWnd, 15, 10, NULL);

    return 0;
  
  case WM_SIZE:
    h = HIWORD(lParam);
    w = LOWORD(lParam);
	
	if (hBm != NULL)
	  DeleteObject(hBm);
    
	hDC = GetDC(hWnd);
	hBm = CreateCompatibleBitmap(hDC, w, h);
	ReleaseDC(hWnd, hDC);
	SelectObject(hMemDC, hBm);

	GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
	
	return 0;
  
  case WM_TIMER:
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

	DrawEye(hMemDC, w / 4, h / 2, 200, 100, pt.x, pt.y);
    DrawEye(hMemDC, w * 3 / 4, h / 2, 200, 100, pt.x, pt.y);
	

	InvalidateRect(hWnd, NULL, FALSE);

    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
	BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
	EndPaint(hWnd, &ps);

	return 0;

  case WM_ERASEBKGND:
    return 0;

  case WM_DESTROY:
    DeleteObject(hBm);
	DeleteDC(hMemDC);
	KillTimer(hWnd, 15);
	PostQuitMessage(0);

    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
