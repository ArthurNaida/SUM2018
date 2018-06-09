#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "rnd.h"

BOOL AN6_RndPrimCreate( an6PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;

  /* Set all primitive data fields to 0 */
  memset(Pr, 0, sizeof(an6PRIM));

  /* Calculate memory size for primitive data */
  size = sizeof(an6VERTEX) * NoofV + sizeof(INT) * NoofI;

  /* Allocate memory */
  Pr->V = malloc(size);
  if (Pr->V == NULL)
    return FALSE;

  /* Fill all allocated memory by 0 */
  memset(Pr->V, 0, size);

  /* Set index array pointer */
  Pr->I = (INT *)(Pr->V + NoofV);

  /* Store data sizes */
  Pr->NumOfV = NoofV;
  Pr->NumOfI = NoofI;
  /* Set default transform (identity) */
  Pr->Trans = MatrIdentity();

  return TRUE;
} /* End of 'AN6_RndPrimCreate' function */

VOID AN6_RndPrimFree( an6PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  /* Set all primitive data fields to 0 */
  memset(Pr, 0, sizeof(an6PRIM));
} /* End of 'AN6_RndPrimFree' function */

VOID AN6_RndPrimDraw( an6PRIM *Pr, MATR World );
{
  INT i;
  POINT *pnts; /* vertex projections */
  MATR M = MatrMulMatr(MatrMulMatr(Pr->Trans, World), AN6_RndMatrVP);

  /* Allocate memory for projections */
  pnts = malloc(sizeof(POINT) * Pr->NumOfV);
  if (pnts == NULL)
    return;

  /* Project all vertices */
  for (i = 0; i < Pr->NumOfV; i++)
  {
    /* Convert from World to NDC */
    VEC p = VecMulMatr4x4(Pr->V[i].P, M);

    /* Convert from World to NDC */
    pnts[i].x = (p.X + 1) * AN6_RndFrameW / 2;
    pnts [i].y = (-p.Y + 1) * AN6_RndFrameH / 2;
  }

  /* Draw all triangles */
  for (i = 0; i < Pr->NumOfI; i += 3)
  {
    POINT p[3];

    p[0] = pnts[Pr->I[i]];
    p[1] = pnts[Pr->I[i + 1]];
    p[2] = pnts[Pr->I[i + 2]];
    Polygon(AN6_hDCRndFrame, p, 3);
  }

  free(pnts);
} /* End of 'VG4_RndPrimDraw' function */

BOOL AN6_RndPrimLoad( an6PRIM *Pr, CHAR *FileName )
{
  INT nv, nf;
  FILE *F;
  CHAR Buf[1000];

  /* Set all primitive data fields to 0 */
  memset(Pr, 0, sizeof(an6PRIM));
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertex and facet quantity */
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;

  /* Create primitive */
  if (!AN6_RndPrimCreate(Pr, nv, nf))
  {
    fclose(F);
    return FALSE;
  }

  /* Load primitive data */
  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      Pr->V[nv++] = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n1, n2, n3;

      sscanf(Buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d//%*d %d//%*d %d//%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d/%*d %d/%*d %d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d %d %d", &n1, &n2, &n3);
      Pr->I[nf++] = n1 - 1;
      Pr->I[nf++] = n2 - 1;
      Pr->I[nf++] = n3 - 1;
    }
  fclose(F);
  return TRUE;
} /* End of 'VG4_RndPrimLoad' function */
