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
/* Texture stock maximum size */
#define AN6_MAX_TEXTURES 30



extern FLT AN6_RndProjSize, AN6_RndProjDist, AN6_RndProjFarClip;
extern INT AN6_RndProgId, AN6_RndTexturesSize, AN6_RndMaterialsSize;
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

  /* Illumination coefficients */    
  VEC Ka, Kd, Ks;         /* Ambient, diffuse, specular coefficients */
  FLT Ph;                 /* Phong power coefficient */
  FLT Trans;              /* Transparency factor */
  INT Tex[8];             /* Texture references from texture table */

  /* Shader string */
  CHAR ShaderStr[AN6_STR_MAX];

  INT ShdNo;              /* Shader number in shader array */


} an6MATERIAL;

typedef struct tagan6SHADER
{
  CHAR Name[AN6_STR_MAX]; /* Shader filename prefix */
  INT ProgId;             /* Shader program Id */
} an6SHADER;

typedef struct tagan6PRIMS
{
  an6PRIM *Prims; /* Primitive array */
  INT NumOfPrims; /* Primitive array size */
  MATR Trans;     /* All primitives transformation matrix */
} an6PRIMS; 

typedef struct tagan6IMAGE
{
  INT W, H;    /* Image size in pixels */
  DWORD *Bits; /* Image pixels: pixel(X,Y) = Bits[Y * W + X] */

  HBITMAP hBm; /* WinAPI bitmap handle */
} an6IMAGE;

typedef struct tagan6TEXTURE
{
  CHAR Name[AN6_STR_MAX]; /* Texture name */
  INT W, H;                          /* Texture size in pixels */
  UINT TexId;                        /* OpenGL texture Id */ 
} an6TEXTURE;

extern an6TEXTURE AN6_RndTextures[AN6_MAX_TEXTURES];
extern an6MATERIAL AN6_RndMaterials[AN6_MAX_MATERIALS];

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

BOOL AN6_RndPrimsCreate( an6PRIMS *Prs, INT NoofP );
VOID AN6_RndPrimsFree( an6PRIMS *Prs );
VOID AN6_RndPrimsDraw( an6PRIMS *Prs, MATR World );
BOOL AN6_RndPrimsLoad( an6PRIMS *Prs, CHAR *FileName );

VOID AN6_RndTexInit( VOID );
VOID AN6_RndTexClose( VOID );
INT AN6_RndTexAddImg( CHAR *Name, INT W, INT H, DWORD *Bits );
INT AN6_RndTexAdd( CHAR *FileName );

BOOL AN6_RndImgLoad( an6IMAGE *Img, CHAR *FileName );
VOID AN6_RndImgFree( an6IMAGE *Img );
VOID AN6_RndImgFree( an6IMAGE *Img );
VOID AN6_RndImgToTex( an6IMAGE *Img );
DWORD AN6_RndImgGet( an6IMAGE *Img, INT X, INT Y );


#endif 






