#include <stdio.h>

#include "..\anim.h"

BOOL AN6_RndPrimCreate( an6PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;

  /* Set all primitive data fields to 0 */
  memset(Pr, 0, sizeof(an6PRIM));

  /* Calculate memory size for primitive data */
  size = sizeof(an6VERTEX) * NoofV + sizeof(INT) * NoofI * 3;

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

VOID AN6_RndPrimDraw( an6PRIM *Pr, MATR World )
{
  INT i;
  MATR M = MatrMulMatr(MatrMulMatr(Pr->Trans, World), AN6_RndMatrVP);

  /* Set transform matrix */
  glLoadMatrixd(M.M[0]);

  /* Draw all triangles */
  glBegin(GL_TRIANGLES);
  for (i = 0; i < Pr->NumOfI;)
  {
    glVertex3dv(&Pr->V[Pr->I[i * 3]].P.x);
    /*
    glVertex3d(Pr->V[Pr->I[i]].P.X,
               Pr->V[Pr->I[i]].P.Y,
               Pr->V[Pr->I[i]].P.Z);
    */
    glVertex3dv(&Pr->V[Pr->I[i * 3 + 1]].P.x);
    glVertex3dv(&Pr->V[Pr->I[i * 3 + 2]].P.x);
    i++;
  }
  glEnd();/* End of 'AN6_RndPrimDraw' function */
}

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
      Pr->V[nv++].P = VecSet(x, y, z);
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
} /* End of 'AN6_RndPrimLoad' function */
