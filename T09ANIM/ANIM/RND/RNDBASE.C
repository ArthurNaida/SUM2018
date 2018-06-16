#include "..\anim.h"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
/*#pragma comment(lib, "glew32s")  */

VOID AN6_RndInit( HWND hWnd )
{ 
  INT i;
  PIXELFORMATDESCRIPTOR pfd = {0};

  /* Store window and context handles */
  AN6_Anim.hWnd = hWnd;
  AN6_Anim.hDC = GetDC(AN6_Anim.hWnd);

  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(AN6_Anim.hDC, &pfd);
  DescribePixelFormat(AN6_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(AN6_Anim.hDC, i, &pfd);

  /* OpenGL init: setup rendering context */
  AN6_Anim.hRC = wglCreateContext(AN6_Anim.hDC);
  wglMakeCurrent(AN6_Anim.hDC, AN6_Anim.hRC);

  /* Set default render parameters */
  glClearColor(0.3, 0.5, 0.7, 1);
  glEnable(GL_DEPTH_TEST);
}

VOID AN6_RndClose( VOID )
{
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(AN6_Anim.hRC);
  ReleaseDC(AN6_Anim.hWnd, AN6_Anim.hDC);
}

VOID AN6_RndResize( INT w, INT h )
{
  glViewport(0, 0, w, h);

  AN6_Anim.W = w;
  AN6_Anim.H = h;

  AN6_RndProjSet();
}

VOID AN6_RndStart( VOID )
{
  /* Clear frame */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

VOID AN6_RndEnd( VOID )
{
  glFinish();
}

VOID AN6_RndCopyFrame( HDC hDC )
{
  SwapBuffers(AN6_Anim.hDC);
}

DBL AN6_RndProjSize = 0.1, AN6_RndProjDist = 0.1, AN6_RndProjFarClip = 30000;
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