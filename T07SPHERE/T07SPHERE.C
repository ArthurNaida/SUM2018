#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <time.h>
#define WND_CLASS_NAME "My window class"
#define PI 3.14159265358979323846
#define N 27
#define M 30

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

  hWnd = CreateWindow(WND_CLASS_NAME, "SPHERE",
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

static VECT S[N][M];

VECT VecCor( DOUBLE x, DOUBLE y, DOUBLE z )
{
  VECT v = {x, y, z};

  return v;
}

VECT RotateZ( VECT v, DOUBLE angle )
{
  return VecCor(v.x * cos(angle * PI / 180) - v.y * sin(angle * PI / 180), v.x * sin(angle * PI / 180) + v.y * cos(angle * PI / 180), v.z);
}

VECT RotateY( VECT v, DOUBLE angle )
{
  return VecCor(v.z * sin(angle * PI / 180) + v.x * cos(angle * PI / 180), v.y, v.z * cos(angle * PI / 180) - v.x * sin(angle * PI / 180));
}

VOID MakeSphere( INT r )
{
  INT i, j;

  for (i = 0; i < N; i++)
  {
    DOUBLE te = i * PI / N;
    for (j = 0; j < M; j++)
    {
      DOUBLE phi = j * 2 * PI / (M - 1);

      S[i][j] = VecCor(r * sin(phi) * sin(te), r * cos(te), r * cos(phi) * sin(te));
    }
  }
    
}

VOID DrawSphere( HDC hDC, INT x, INT y, INT r )
{
  DOUBLE t = clock() / (DOUBLE)CLOCKS_PER_SEC, Sf;
  INT i, j;
  static POINT P[N][M];

  for (i = 0; i < N; i++)
  {
    for (j = 0; j < M; j++)
    {
      VECT v = RotateY(RotateZ(S[i][j], 20 * t), 13 * t);
      P[i][j].x = x + r * v.x;
      P[i][j].y = y - r * v.y;
    }
  }

  for (i = 0; i < N - 1; i++)
    for (j = 0; j < M - 1; j++)
    {
      POINT pts[4];

      pts[0] = P[i][j];
      pts[1] = P[i][j + 1];
      pts[2] = P[i + 1][j + 1];
      pts[3] = P[i + 1][j];

      Sf = (pts[0].x - pts[1].x) * (pts[1].y + pts[0].y) +
        (pts[1].x - pts[2].x) * (pts[2].y + pts[1].y) +
        (pts[2].x - pts[3].x) * (pts[3].y + pts[2].y) +
        (pts[3].x - pts[0].x) * (pts[0].y + pts[3].y);
      
        if (Sf > 0)
        {
          SetDCPenColor(hDC, RGB(0, 0, 0));
          SetDCBrushColor(hDC, RGB(150, 150, 0));
          Polygon(hDC, pts, 4);
        }
        else
        {
          SetDCPenColor(hDC, RGB(0, 0, 0));
          SetDCBrushColor(hDC, RGB(130, 130, 0));
          Polygon(hDC, pts, 4);
        }
       
      SetDCPenColor(hDC, RGB(0, 0, 0));
      SetDCBrushColor(hDC, RGB(150, 150, 0));
      Polygon(hDC, pts, 4);


    }

  
}

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
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);

    ReleaseDC(hWnd, hDC);



    SetTimer(hWnd, 15, 10, NULL);

    return 0;
  
  case WM_SIZE:
    h = HIWORD(lParam);
    w = LOWORD(lParam);
    
    hDC = GetDC(hWnd);
    if (hBm != NULL)
      DeleteObject(hBm);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    return 0;

  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
 
  case WM_TIMER:
 
    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SelectObject(hMemDC, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDC, RGB(0, 0, 0));
    SetDCBrushColor(hMemDC, RGB(0, 0, 0));
    Rectangle(hMemDC, 0, 0, w, h);

    SelectObject(hMemDC, GetStockObject(DC_BRUSH));
    SelectObject(hMemDC, GetStockObject(DC_PEN));
    SetDCPenColor(hMemDC, RGB(150, 150, 0));
    SetDCBrushColor(hMemDC, RGB(0, 0, 0));
    MakeSphere(15);
    DrawSphere(hMemDC, w / 2, h / 2, 20);
    InvalidateRect(hWnd, NULL, TRUE);

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