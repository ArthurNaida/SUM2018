#ifndef __RND_H_
#define __RND_H_

#include "../../def.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

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
VOID AN6_RndCamSet( VEC Loc, VEC At, VEC Up1 );

BOOL AN6_RndPrimCreate( an6PRIM *Pr, INT NoofV, INT NoofI );
VOID AN6_RndPrimFree( an6PRIM *Pr );
VOID AN6_RndPrimDraw( an6PRIM *Pr, MATR World );
BOOL AN6_RndPrimLoad( an6PRIM *Pr, CHAR *FileName );

#endif 






