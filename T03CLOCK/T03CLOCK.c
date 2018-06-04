#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <time.h>
#define WND_CLASS_NAME "My window class"
#define PI 3.14159265358979323846

INT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

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

  hWnd = CreateWindow(WND_CLASS_NAME, "CLOCK",
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


void DrawClock( HDC hDC, INT x, INT y, INT len )
{
  SYSTEMTIME tm;
  static INT w, h;

  GetLocalTime(&tm);

  MoveToEx(hDC, x, y, NULL);
  LineTo(hDC, x + len * sin(tm.wSecond * PI / 30 + tm.wMilliseconds * PI / 30000), y - len * cos(tm.wSecond * PI / 30 + tm.wMilliseconds * PI / 30000));

  MoveToEx(hDC, x, y, NULL);
  LineTo(hDC, x + len * 2 / 3 * sin(tm.wMinute * PI / 30 + tm.wSecond * PI / 30000), y - len * 2 / 3 * cos(tm.wMinute * PI / 30 + tm.wSecond * PI / 30000));

  MoveToEx(hDC, x, y, NULL);
  LineTo(hDC, x + len / 3 * sin(tm.wHour * PI / 30 + tm.wMinute * PI / 30000), y - len / 3 * cos(tm.wHour * PI / 30 + tm.wMinute * PI / 30000));
}                                                     

INT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  static INT w, h;
  static HDC hMemDC, hDCClock;
  static HBITMAP hBm, hBmClock;
  static HFONT hFnt;
  INT l;
  SIZE s;
  CHAR Buf[50];
  POINT pt;
  SYSTEMTIME tm;
  PAINTSTRUCT ps;
  HDC hDC;
  BITMAP bm;

  GetCursorPos(&pt);
  ScreenToClient(hWnd, &pt);

  switch (Msg)
  {
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    hDCClock = CreateCompatibleDC(hDC);
    hFnt = CreateFont(50, 0, 30, 0, FW_NORMAL, FALSE, TRUE, TRUE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_ROMAN, "");
  
    ReleaseDC(hWnd, hDC);

    hBmClock = LoadImage(NULL, "CLOCKFACE.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(hDCClock, hBmClock);

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
	
    return 0;
 
  case WM_TIMER:
    GetObject(hBmClock, sizeof(BITMAP), &bm);
    BitBlt(hMemDC, (w - bm.bmWidth) / 2, (h - bm.bmHeight) / 2, bm.bmWidth, bm.bmHeight, hDCClock, 0, 0, SRCCOPY);

    DrawClock(hMemDC, w / 2, h / 2, bm.bmWidth / 2);

    GetLocalTime(&tm);
    l = sprintf(Buf, "%02.i day, %02.i.%02.i.%02.i", tm.wDayOfWeek, tm.wHour, tm.wMinute, tm.wSecond);
    GetTextExtentPoint(hMemDC, Buf, l, &s);
    TextOut(hMemDC, w / 2 - 50, bm.bmHeight + (h - bm.bmHeight) / 2, Buf, l);

    InvalidateRect(hWnd, NULL, FALSE);

    return 0;

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