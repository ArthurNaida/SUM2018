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
    0
  };

  AN6_RndMaterialsSize = 0;
  AN6_RndMtlAdd(&defm);
} /* End of 'AN6_RndMtlInit' function */

/* Add material to stock function.
 * ARGUMENTS:
 *   - pointer to material data:
 *       vg4MATERIAL *Mtl;
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
  INT prg, loc;
  an6MATERIAL *mtl;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= AN6_RndMaterialsSize)
    MtlNo = 0;
  mtl = &AN6_RndMaterials[MtlNo];

  /* Set program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= AN6_RndShadersSize)
    prg = 0;
  else
    prg = AN6_RndShaders[prg].ProgId;

  glUseProgram(prg);
  if ((loc = glGetUniformLocation(prg, "Time")) != -1)
    glUniform1f(loc, AN6_Anim.Time);
  if ((loc = glGetUniformLocation(prg, "GlobalTime")) != -1)
    glUniform1f(loc, AN6_Anim.GlobalTime);

  return prg;
} /* End of 'AN6_RndMtlApply' function */

/* END OF 'RNDMTL.C' FILE */