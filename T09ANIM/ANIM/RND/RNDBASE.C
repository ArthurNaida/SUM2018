#include "..\anim.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32s")

VOID AN6_RndInit( HWND hWnd )
{ 
  HDC hDC;

  hDC = GetDC(AN6_Anim.hWnd);
  AN6_Anim.hDC = CreateCompatibleDC(hDC);
  ReleaseDC(AN6_Anim.hWnd, AN6_Anim.hDC);

  AN6_RndCamSet(VecSet(30, 30, 30), VecSet(0, 0, 0), VecSet(0, 1, 0));
}

VOID AN6_RndClose( VOID )
{
  DeleteObject(AN6_Anim.hBm);
  DeleteObject(AN6_Anim.hDC);
}

VOID AN6_RndResize( INT w, INT h )
{
  HDC hDC;

  if (AN6_Anim.hBm != NULL)
    DeleteObject(AN6_Anim.hBm);
  hDC = GetDC(AN6_Anim.hWnd);
  AN6_Anim.hBm = CreateCompatibleBitmap(hDC, w, h);
  ReleaseDC(AN6_Anim.hWnd, hDC);
  SelectObject(AN6_Anim.hDC, AN6_Anim.hBm);

  AN6_Anim.W = w;
  AN6_Anim.H = h;

  AN6_RndProjSet();
}

VOID AN6_RndStart( VOID )
{
  SelectObject(AN6_Anim.hDC, GetStockObject(WHITE_BRUSH));
  SelectObject(AN6_Anim.hDC, GetStockObject(NULL_PEN));

  Rectangle(AN6_Anim.hDC, 0, 0, AN6_Anim.W, AN6_Anim.H);

  SelectObject(AN6_Anim.hDC, GetStockObject(NULL_BRUSH));
  SelectObject(AN6_Anim.hDC, GetStockObject(DC_PEN));

  SetDCPenColor(AN6_Anim.hDC, RGB(0, 0, 0));
}

VOID AN6_RndEnd( VOID )
{
}

VOID AN6_RndCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, AN6_Anim.W, AN6_Anim.H, AN6_Anim.hDC, 0, 0, SRCCOPY);
}

DBL AN6_RndProjSize = 0.1, AN6_RndProjDist = 0.1, AN6_RndProjFarClip = 300;
MATR  AN6_RndMatrView, AN6_RndMatrProj, AN6_RndMatrVP;

VOID AN6_RndProjSet( VOID )
{
  DBL ratio_x, ratio_y;

  ratio_x = ratio_y = AN6_RndProjSize / 2;
  if (AN6_Anim.W > AN6_Anim.H) 
    ratio_x *= (DBL)AN6_Anim.W / AN6_Anim.H;
  else
    ratio_y *= (DBL)AN6_Anim.H / AN6_Anim.W;

  AN6_RndMatrProj = MatrFrustum(-ratio_x, ratio_x, -ratio_y, ratio_y, AN6_RndProjDist, AN6_RndProjFarClip);
  AN6_RndMatrVP = MatrMulMatr(AN6_RndMatrView, AN6_RndMatrProj);
}

VOID AN6_RndCamSet( VEC Loc, VEC At, VEC Up1 )
{
  AN6_RndMatrView = MatrView(Loc, At, Up1);
  AN6_RndMatrVP = MatrMulMatr(AN6_RndMatrView, AN6_RndMatrProj);
}