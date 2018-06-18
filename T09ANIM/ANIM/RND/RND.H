#ifndef __RND_H_
#define __RND_H_

#include "../../def.h"

#define GLEW_STATIC
#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>
#define AN6_MAX_MATERIALS 30
#define AN6_STR_MAX 300
#define AN6_MAX_SHADERS 30



extern FLT AN6_RndProjSize, AN6_RndProjDist, AN6_RndProjFarClip;
extern INT AN6_RndProgId;
extern MATR  AN6_RndMatrView, AN6_RndMatrProj, AN6_RndMatrVP;

typedef struct tagan6VERTEX
{
  VEC P;  /* Vertex position */
} an6VERTEX;

typedef enum tagan6PRIM_TYPE
{
  AN6_RND_PRIM_TRIMESH, AN6_RND_PRIM_FRID
} an6PRIM_TYPE;

typedef struct tagan6PRIM
{
  an6PRIM_TYPE Type; /* Primitive type */
  an6VERTEX *V; /* Vertex attributes array */
  INT NumOfV;   /* Number of vertices */
  INT VA;
  INT VBuf;
  INT IBuf;
  INT MtlNo;
  INT *I;       /* Index array (for trimesh – by 3 ones) */
  INT NumOfI;   /* Number of indices */
  MATR Trans;   /* Additional transformation matrix */

} an6PRIM;

typedef struct tagan6MATERIAL
{
  CHAR Name[AN6_STR_MAX]; /* Material name */
  INT ShdNo;              /* Shader number in shader array */

} an6MATERIAL;

typedef struct tagan6SHADER
{
  CHAR Name[AN6_STR_MAX]; /* Shader filename prefix */
  INT ProgId;             /* Shader program Id */
} an6SHADER;

an6SHADER AN6_RndShaders[AN6_MAX_SHADERS]; /* Array of shaders */
INT AN6_RndShadersSize;                    /* Shadres array store size */

VOID AN6_RndInit( HWND hWnd );
VOID AN6_RndResize( INT w, INT h );
VOID AN6_RndClose( VOID );
VOID AN6_RndStart( VOID );
VOID AN6_RndEnd( VOID );
VOID AN6_RndCopyFrame( HDC hDC );
VOID AN6_RndProjSet( VOID );
VOID AN6_RndCamSet( VEC Loc, VEC At, VEC Up1 );

BOOL AN6_RndPrimCreate( an6PRIM *Pr, an6PRIM_TYPE Type, an6VERTEX *V, INT NoofV, INT *I, INT NoofI );
VOID AN6_RndPrimFree( an6PRIM *Pr );
VOID AN6_RndPrimDraw( an6PRIM *Pr, MATR World );
BOOL AN6_RndPrimLoad( an6PRIM *Pr, CHAR *FileName );

VOID AN6_RndMtlInit( VOID );
INT AN6_RndMtlAdd( an6MATERIAL *Mtl );
INT AN6_RndMtlApply( INT MtlNo );
INT AN6_RndShdLoad( CHAR *ShaderFileNamePrefix );
VOID AN6_RndShdDelete( INT ProgId );
VOID AN6_RndShdInit( VOID );
VOID AN6_RndShdClose( VOID );
INT AN6_RndShdAdd( CHAR *ShaderFileNamePrefix );
VOID AN6_RndShdUpdate( VOID );

#endif 






