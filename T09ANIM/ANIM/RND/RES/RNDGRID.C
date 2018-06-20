 /* FILE NAME   : RNDGRID.C
 * PURPOSE     : WinAPI animation system.
 *               Rendering system implementation module.
 *               Grid topology handle.
 * LAST UPDATE : 19.06.2018.
 * NOTE        : Module prefix 'AN6'.
 */

#include <string.h>

#include "../../anim.h"

/* Convert grid to primitive function.
 * ARGUMENTS:
 *   - primitive to be constructed:
 *       an6PRIM *Pr;
 *   - source grid:
 *       an6GRID *G;
 * RETURNS: None.
 */
VOID AN6_RndPrimFromGrid( an6PRIM *Pr, an6GRID *G )
{
  INT i, j, k, v, *Ind;

  memset(Pr, 0, sizeof(an6PRIM));

  /* Index array allocation */
  if ((Ind = malloc(((G->H - 1) * (G->W * 2 + 1) - 1) * sizeof(INT))) == NULL)
    return;
  /* Index array fill */
  for (i = 0, k = 0, v = 0; i < G->H - 1; i++)
  {
    for (j = 0; j < G->W; j++)
    {
      Ind[k++] = v + G->W;
      Ind[k++] = v++;
    }
    if (i < G->H - 2)
      Ind[k++] = -1;
  }
  /* Create primitive */
  AN6_RndPrimCreate(Pr, AN6_RND_PRIM_GRID,
    G->V, G->W * G->H, Ind, (G->H - 1) * (G->W * 2 + 1) - 1);
  free(Ind);
} /* End of 'AN6_RndPrimfFromGrid' function */

/* Grid creation function.
 * ARGUMENTS:
 *   - grid to be create:
 *       an6GRID *G;
 *   - grid size:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL AN6_RndGridCreate( an6GRID *G, INT W, INT H )
{
  INT i, j, v;

  memset(G, 0, sizeof(an6GRID));
  if ((G->V = malloc(W * H * sizeof(an6VERTEX))) == NULL)
    return FALSE;
  /* Setup all vertices */
  for (i = 0, v = 0; i < H; i++)
    for (j = 0; j < W; j++, v++)
    {
      G->V[v].P = VecSet(i, 0, j);
      G->V[v].T = Vec2Set(j / (W - 1.0), i / (H - 1.0));
      G->V[v].N = VecSet(0, 1, 0);
      G->V[v].C = Vec4Set(1, 1, 1, 1);
    }
  G->W = W;
  G->H = H;
  return TRUE;
} /* End of 'AN6_RndGridCreate' function */

/* Grid free function.
 * ARGUMENTS:
 *   - grid to free:
 *       an6GRID *G;
 * RETURNS: None.
 */
VOID AN6_RndGridFree( an6GRID *G )
{
  if (G->V != NULL)
    free(G->V);
  memset(G, 0, sizeof(an6GRID));
} /* End of 'AN6_RndGridFree' function */

/* Grid normal evaluation function.
 * ARGUMENTS:
 *   - grid for recalculate normals:
 *       an6GRID *G;
 * RETURNS: None.
 */
VOID AN6_RndPrimGridEvalNormals( an6GRID *G )
{
  INT i, j, k;

  /* Set all vertex normals to (0, 0, 0) */
  for (i = 0, k = 0; i < G->H; i++)
    for (j = 0; j < G->W; j++)
      G->V[k++].N = VecSet(0, 0, 0);

  /* Evaluate all triangles normals */
  for (i = 0, k = 0; i < G->H - 1; i++)
    for (j = 0; j < G->W - 1; j++, k++)
    {
      an6VERTEX
        *p00 = &G->V[k],
        *p01 = &G->V[k + 1],
        *p10 = &G->V[k + G->W],
        *p11 = &G->V[k + G->W + 1];
      VEC N;

      N = VecNormalize(VecCrossVec(VecSubVec(p00->P, p10->P), VecSubVec(p11->P, p10->P)));
      p00->N = VecAddVec(p00->N, N);
      p10->N = VecAddVec(p10->N, N);
      p11->N = VecAddVec(p11->N, N);

      N = VecNormalize(VecCrossVec(VecSubVec(p11->P, p01->P), VecSubVec(p00->P, p01->P)));
      p00->N = VecAddVec(p00->N, N);
      p01->N = VecAddVec(p01->N, N);
      p11->N = VecAddVec(p11->N, N);
    }

  /* Normalize all vertex normals */
  for (i = 0; i < G->H * G->W; i++)
    G->V[i].N = VecNormalize(G->V[i].N);
} /* End of 'AN6_RndPrimGridEvalNormals' function */

/* Sphere grid creation function.
 * ARGUMENTS:
 *   - grid to create:
 *       an6GRID *G;
 *   - grid size:
 *       INT W, H;
 *   - sphere center:
 *       VEC C;
 *   - sphere radius:
 *       FLT R;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL AN6_RndGridCreateSphere( an6GRID *G, INT W, INT H, VEC C, FLT R )
{
  INT i, j, v;

  if (!AN6_RndGridCreate(G, W, H))
    return FALSE;
  /* Setup all vertices */
  for (i = 0, v = 0; i < H; i++)
  {
    DBL theta = (H - 1 - i) * PI / (H - 1), sit = sin(theta), cot = cos(theta);

    for (j = 0; j < W; j++, v++)
    {
      DBL
        phi = j * 2 * PI / (W - 1), sip = sin(phi), cop = cos(phi),
        x = sit * sip,
        y = cot,
        z = sit * cop;

      G->V[v].P = VecSet(C.x + x * R, C.y + y * R, C.z + z * R);
      G->V[v].N = VecSet(x, y, z);
    }
  }
  return TRUE;
} /* End of 'AN6_RndGridCreateSphere' function */

/* Plane grid creation function.
 * ARGUMENTS:
 *   - grid to create:
 *       an6GRID *G;
 *   - grid size:
 *       INT W, H;
 *   - plane corner and directional vectors:
 *       VEC P, Du, Dv;
 *   - plane texture scale factors:
 *       FLT Tx, Ty;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL AN6_RndGridCreatePlane( an6GRID *G, INT W, INT H, VEC P, VEC Du, VEC Dv, FLT Tx, FLT Ty )
{
  INT i, j, v;
  VEC N;

  if (!AN6_RndGridCreate(G, W, H))
    return FALSE;

  N = VecNormalize(VecCrossVec(Du, Dv));

  /* Setup all vertices */
  for (i = 0, v = 0; i < H; i++)
  {
    DBL sy = i / (H - 1.0);

    for (j = 0; j < W; j++, v++)
    {
      DBL sx = j / (W - 1.0);

      G->V[v].P = VecAddVec(P,
        VecAddVec(VecMulNum(Du, sx), VecMulNum(Dv, sy)));
      G->V[v].N = N;
      G->V[v].T.x *= Tx;
      G->V[v].T.y *= Ty;
    }
  }
  return TRUE;
} /* End of 'AN6_RndGridCreatePlane' function */

/* Height field grid creation function.
 * ARGUMENTS:
 *   - grid to create:
 *       an6GRID *G;
 *   - height field image file name:
 *       CHAR *HeightFieldFileName;
 *   - plane corner and directional vectors:
 *       VEC P, Du, Dv;
 *   - height scale factor:
 *       FLT Scale;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL AN6_RndGridCreateHeightField( an6GRID *G, CHAR *HeightFieldFileName, VEC P, VEC Du, VEC Dv, FLT Scale )
{
  INT i, j, v;
  an6IMAGE Img;
  VEC N;

  memset(G, 0, sizeof(an6GRID));
  if (!AN6_RndImgLoad(&Img, HeightFieldFileName))
    return FALSE;

  if (!AN6_RndGridCreatePlane(G, Img.W, Img.H, P, Du, Dv, 1, 1))
  {
    AN6_RndImgFree(&Img);
    return FALSE;
  }

  N = G->V[0].N;

  /* Setup all vertices */
  for (i = 0, v = 0; i < G->H; i++)
  {
    for (j = 0; j < G->W; j++, v++)
    {
      DWORD col = AN6_RndImgGet(&Img, j, i);

      G->V[v].P = VecAddVec(G->V[v].P,
        VecMulNum(N, (((col >> 8) & 0xFF) / 255.0 * 2 - 1) * Scale));
    }
  }
  AN6_RndImgFree(&Img);
  AN6_RndPrimGridEvalNormals(G);
  return TRUE;
} /* End of 'AN6_RndGridCreateHeightField' function */

/* END OF 'RNDGRID.C' FILE */