#include <stdio.h>

#include "..\anim.h"

BOOL AN6_RndPrimCreate( an6PRIM *Pr, an6PRIM_TYPE Type, an6VERTEX *V, INT NoofV, INT *I, INT NoofI )
{
  /* Set all primitive data fields to 0 */
  memset(Pr, 0, sizeof(an6PRIM));

  glGenBuffers(1, &Pr->VBuf);
  glGenVertexArrays(1, &Pr->VA);

  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
  glBufferData(GL_ARRAY_BUFFER, sizeof(an6VERTEX) * NoofV, V, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(an6VERTEX), (VOID *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(an6VERTEX), (VOID *)sizeof(VEC));
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(an6VERTEX), (VOID *)(sizeof(VEC) + sizeof(VEC2)));
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(an6VERTEX), (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  glBindVertexArray(0);

  if (NoofI != 0)
  {
    glGenBuffers(1, &Pr->IBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, I, GL_STATIC_DRAW);
    Pr->NumOfI = NoofI;
  }
  else
    Pr->NumOfI = NoofV;

  Pr->Type = Type;

  /* Set default transform (identity) */
  Pr->Trans = MatrIdentity();

  return TRUE;
} /* End of 'AN64_RndPrimCreate' function */

VOID AN6_RndPrimFree( an6PRIM *Pr )
{
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);

  if (Pr->IBuf != 0)
    glDeleteBuffers(1, &Pr->IBuf);

  /* Set all primitive data fields to 0 */
  memset(Pr, 0, sizeof(an6PRIM));
} /* End of 'AN6_RndPrimFree' function */

VOID AN6_RndPrimDraw( an6PRIM *Pr, MATR World )
{
  INT AN6_RndProgId;
  INT loc;
  INT gl_prim_type;
  MATR WVP = MatrMulMatr(World, MatrMulMatr(AN6_RndMatrView, AN6_RndMatrProj));

  /* Set transform matrix */
  glLoadMatrixf(WVP.M[0]);

  /* Apply material */
  AN6_RndProgId = AN6_RndMtlApply(Pr->MtlNo);

  if ((loc = glGetUniformLocation(AN6_RndProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, WVP.M[0]);
  if ((loc = glGetUniformLocation(AN6_RndProgId, "Time")) != -1)
    glUniform1f(loc, AN6_Anim.Time);

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);

  gl_prim_type = Pr->Type == AN6_RND_PRIM_TRIMESH ? GL_TRIANGLES : GL_TRIANGLE_STRIP;
  glBindVertexArray(Pr->VA);
  if (Pr->IBuf == 0)
    glDrawArrays(gl_prim_type, 0, Pr->NumOfI);
  else
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glDrawElements(gl_prim_type, Pr->NumOfI, GL_UNSIGNED_INT, NULL);
  }
  glBindVertexArray(0);
  glUseProgram(0);
}

BOOL AN6_RndPrimLoad( an6PRIM *Pr, CHAR *FileName )
{
  INT nv, nf, size, i = 0;
  an6VERTEX *V;
  INT *I;
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
      nf += 3;

  /* Calculate memory size for primitive data */
  size = sizeof(an6VERTEX) * nv + sizeof(INT) * nf;

  /* Allocate memory */
  V = malloc(size);
  if (V == NULL)
  {
    fclose(F);
    return FALSE;
  }

  /* Fill all allocated memory by 0 */
  memset(V, 0, size);
  I = (INT *)(V + nv);

  /* Load primitive data */
  rewind(F);
  nv = nf = 0;
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      FLT x, y, z;

      sscanf(Buf + 2, "%f%f%f", &x, &y, &z);
      V[nv++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n1, n2, n3;

      sscanf(Buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d//%*d %d//%*d %d//%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d/%*d %d/%*d %d/%*d", &n1, &n2, &n3) == 3 ||
        sscanf(Buf + 2, "%d %d %d", &n1, &n2, &n3);
      I[nf++] = n1 - 1;
      I[nf++] = n2 - 1;
      I[nf++] = n3 - 1;
    }
    i++;
  }
  fclose(F);
  /* Create primitive */
  AN6_RndPrimCreate(Pr, AN6_RND_PRIM_TRIMESH, V, nv, I, nf);
  free(V);
  return TRUE;
} /* End of 'AN6_RndPrimLoad' function */
