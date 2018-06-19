#include "../../anim.h"

/* Array of materials */
an6MATERIAL AN6_RndMaterials[AN6_MAX_MATERIALS];
/* Materials array store size */
INT AN6_RndMaterialsSize;

/* Material storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AN6_RndMtlInit( VOID )
{
  an6MATERIAL defm =
  {
    "Default material",
    {0.2, 0.2, 0.2}, {0.8, 0.8, 0.8}, {0.3, 0.3, 0.3}, 24,
    1,
    {-1, -1, -1, -1, -1, -1, -1, -1},
    "",
    0
  };

  AN6_RndMaterialsSize = 0;
  AN6_RndMtlAdd(&defm);
} /* End of 'AN6_RndMtlInit' function */

/* Add material to stock function.
 * ARGUMENTS:
 *   - pointer to material data:
 *       an6MATERIAL *Mtl;
 * RETURNS:
 *   (INT) new material stock number.
 */
INT AN6_RndMtlAdd( an6MATERIAL *Mtl )
{
  if (AN6_RndMaterialsSize >= AN6_MAX_MATERIALS)
    return 0;
  AN6_RndMaterials[AN6_RndMaterialsSize] = *Mtl;
  return AN6_RndMaterialsSize++;
} /* End of 'AN6_RndMtlAdd' function */

/* Apply material before rendering function.
 * ARGUMENTS:
 *   - material number in materials storage array:
 *       INT MtlNo;
 * RETURNS:
 *   (INT) applyed material shader program Id.
 */
INT AN6_RndMtlApply( INT MtlNo )
{
  INT prg, loc, i;
  an6MATERIAL *mtl;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= AN6_RndMaterialsSize)
    MtlNo = 0;
  mtl = &AN6_RndMaterials[MtlNo];

  /* Set program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= AN6_RndShadersSize)
    prg = AN6_RndShaders[0].ProgId;
  else
    prg = AN6_RndShaders[prg].ProgId;

  glUseProgram(prg);

  if ((loc = glGetUniformLocation(prg, "Time")) != -1)
    glUniform1f(loc, AN6_Anim.Time);
  if ((loc = glGetUniformLocation(prg, "GlobalTime")) != -1)
    glUniform1f(loc, AN6_Anim.GlobalTime);
  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.x);
  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.x);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.x);
  if ((loc = glGetUniformLocation(prg, "Ph")) != -1)
    glUniform1f(loc, mtl->Ph);
  if ((loc = glGetUniformLocation(prg, "Trans")) != -1)
    glUniform1f(loc, mtl->Trans);
  for (i = 0; i < 8; i++)
  {
    CHAR Buf[100] = "IsTexture0";

    Buf[9] = '0' + i;
    if ((loc = glGetUniformLocation(prg, Buf)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);
    if (mtl->Tex[i] != -1)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, AN6_RndTextures[mtl->Tex[i]].TexId);
    }
  }

  return prg;
} /* End of 'AN6_RndMtlApply' function */

/* END OF 'RNDMTL.C' FILE */