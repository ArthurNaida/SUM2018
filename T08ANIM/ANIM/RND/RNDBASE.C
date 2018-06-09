#include <windows.h>
#include "rnd.h"

VOID AN6_RndInit( HWND hWnd )
{ 
  HDC hDC;

  hDC = GetDC(AN6_hWndRnd);
  AN6_hDCRndFrame = CreateCompatibleDC(hDC);
  ReleaseDC(AN6_hWndRnd, AN6_hDCRndFrame);
}

VOID AN6_RndClose( VOID )
{
  DeleteObject(AN6_hBmRndFrame);
  DeleteObject(AN6_hDCRndFrame);
}

VOID AN6_RndResize( INT w, INT h )
{
  HDC hDC;

  if (AN6_hBmRndFrame != NULL)
  DeleteObject(AN6_hBmRndFrame);
  hDC = GetDC(AN6_hWndRnd);
  AN6_hBmRndFrame = CreateCompatibleBitmap(hDC, w, h);
  ReleaseDC(AN6_hWndRnd, hDC);
  SelectObject(AN6_hDCRndFrame, AN6_hBmRndFrame);

  AN6_RndFrameW = w;
  AN6_RndFrameH = h;
  AN6_RndProjSet();
}

VOID AN6_RndStart( VOID )
{
  SelectObject(AN6_hDCRndFrame, GetStockObject(WHITE_BRUSH));
  SelectObject(AN6_hDCRndFrame, GetStockObject(NULL_PEN));

  Rectangle(AN6_hDCRndFrame, 0, 0, AN6_RndFrameW, AN6_RndFrameH);
}

VOID AN6_RndEnd( VOID )
{
}

VOID AN6_RndCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, AN6_RndFrameW, AN6_RndFrameH, AN6_hDCRndFrame, 0, 0, SRCCOPY);
}

DBL AN6_RndProjSize = 0.1, AN6_RndProjDist = 0.1, AN6_RndProjFarClip = 300;
MATR  AN6_RndMatrView, AN6_RndMatrProj, AN6_RndMatrVP;

VOID AN6_RndProjSet( VOID )
{
  DBL ratio_x, ratio_y;

  ratio_x = ratio_y = AN6_RndProjSize / 2;
  if (AN6_RndFrameW > AN6_RndFrameH) 
    ratio_x *= (DBL)AN6_RndFrameW / AN6_RndFrameH;
  else
    ratio_y *= (DBL)AN6_RndFrameH / AN6_RndFrameW;

  AN6_RndMatrProj = MatrFrustum(-ratio_x, ratio_x, -ratio_y, ratio_y, AN6_RndProjDist, AN6_RndProjFarClip);
  AN6_RndMatrVP = MatrMulMatr(AN6_RndMatrView, AN6_RndMatrProj);
}

VOID VG4_RndCamSet( VEC Loc, VEC At, VEC Up1 )
{
  AN6_RndMatrView = MatrView(Loc, At, Up1);
  AN6_RndMatrVP = MatrMulMatr(AN6_RndMatrView, AN6_RndMatrProj);
}