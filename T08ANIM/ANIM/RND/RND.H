#ifndef __RND_H_ 
#define __RND_H_

#include <windows.h>
#include "../../def.h"


extern HWND AN6_hWndRnd;
extern HDC AN6_hDCRndFrame;
extern HBITMAP AN6_hBmRndFrame;
extern INT AN6_RndFrameW, AN6_RndFrameH;
extern DBL AN6_RndProjSize, AN6_RndProjDist, AN6_RndProjFarClip;
extern MATR  AN6_RndMatrView, AN6_RndMatrProj, AN6_RndMatrVP;

typedef struct tagvg4VERTEX
{
  VEC P;  /* Vertex position */
} an6VERTEX;

typedef struct tagan6PRIM
{
  an6VERTEX *V; /* Vertex attributes array */
  INT NumOfV;   /* Number of vertices */
  INT *I;       /* Index array (for trimesh – by 3 ones) */
  INT NumOfI;   /* Number of indices */
  MATR Trans;   /* Additional transformation matrix */
} an6PRIM;

VOID AN6_RndInit( HWND hWnd );
VOID AN6_RndResize( INT w, INT h );
VOID AN6_RndClose( VOID );
VOID AN6_RndStart( VOID );
VOID AN6_RndEnd( VOID );
VOID AN6_RndCopyFrame( HDC hDC );
VOID AN6_RndProjSet( VOID );
VOID VG4_RndCamSet( VEC Loc, VEC At, VEC Up1 );

#endif 






