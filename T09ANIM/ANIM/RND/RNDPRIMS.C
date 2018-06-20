#include <stdio.h>
#include <string.h>

#include "../anim.h"

/* Create array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives array:
 *       an6PRIMS *Prs;
 *   - number of primitives:
 *       INT NoofP;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AN6_RndPrimsCreate( an6PRIMS *Prs, INT NoofP )
{
  memset(Prs, 0, sizeof(an6PRIMS));
  if ((Prs->Prims = malloc(sizeof(an6PRIM) * NoofP)) == NULL)
    return FALSE;
  memset(Prs->Prims, 0, sizeof(an6PRIM) * NoofP);
  Prs->NumOfPrims = NoofP;
  Prs->Trans = MatrIdentity();
  return TRUE;
} /* End of 'AN6_RndPrimsCreate' function */  

/* Delete array of primitives function.
 * ARGUMENTS:
 *   - pointerto primitives array:
 *       an6PRIMS *Prs;
 * RETURNS: None.
 */
VOID AN6_RndPrimsFree( an6PRIMS *Prs )
{
  if (Prs->Prims != NULL)
    free(Prs->Prims);
  memset(Prs, 0, sizeof(an6PRIMS));
} /* End of 'AN6_RndPrimsFree' function */  

/* Draw array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives array:
 *       vg4PRIMS *Prs;
 *   - world transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID AN6_RndPrimsDraw( an6PRIMS *Prs, MATR World )
{
  INT i;
  INT AN6_RndPrimAddons[2];

 // World = MatrMulMatr(MatrMulMatr(Prs->Trans, World), MatrRotateY(AN6_Anim.GlobalTime * 5));
  AN6_RndPrimAddons[0] = Prs->NumOfPrims;

  /* Output non-transparent primitves */
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (Prs->Prims[i].MtlNo >= 0 && Prs->Prims[i].MtlNo < AN6_RndMaterialsSize &&
        AN6_RndMaterials[Prs->Prims[i].MtlNo].Trans >= 1)
    {
      AN6_RndPrimAddons[1] = i;
      
      if (i == 7 || i == 9)
        AN6_RndPrimDraw(&Prs->Prims[i], MatrMulMatr(World, MatrRotateY(AN6_Anim.Time * 1000)));
      else
        AN6_RndPrimDraw(&Prs->Prims[i], World);
    }

  /* Output transparent primitves */
  /* Back face */
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (Prs->Prims[i].MtlNo >= 0 && Prs->Prims[i].MtlNo < AN6_RndMaterialsSize &&
        AN6_RndMaterials[Prs->Prims[i].MtlNo].Trans < 1)
    {
      AN6_RndPrimAddons[1] = i;
    }
  /* Front face */
  glCullFace(GL_BACK);
  for (i = 0; i < Prs->NumOfPrims; i++)
    if (Prs->Prims[i].MtlNo >= 0 && Prs->Prims[i].MtlNo < AN6_RndMaterialsSize &&
        AN6_RndMaterials[Prs->Prims[i].MtlNo].Trans < 1)
    {
      AN6_RndPrimAddons[1] = i;
    }
  glDisable(GL_CULL_FACE);
} /* End of 'AN6_RndPrimsDraw' function */  

/* Load array of primitives from G3DM file function.
 * ARGUMENTS:
 *   - pointer to primitives array:
 * - vg4PRIMS *Prs;
 *   - filename to load from:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL AN6_RndPrimsLoad( an6PRIMS *Prs, CHAR *FileName )                                         
{
  FILE *F;
  INT flen, NoofP, NoofM, NoofT, p, m, t, first_mtl_no, first_tex_no;
  DWORD Sign;
  BYTE *mem, *ptr;

  memset(Prs, 0, sizeof(an6PRIMS));

  /* Load the whole file into memory */
  if ((F = fopen(FileName, "rb")) == NULL)
    return FALSE;
  fseek(F, 0, SEEK_END);
  flen = ftell(F); /* measure file length */
  rewind(F);

  /* Allocate memory */
  if ((mem = malloc(flen)) == NULL)
  {
    fclose(F);
    return FALSE;
  }
  /* Read data */
  fread(mem, 1, flen, F);
  fclose(F);

  /* Setup file pointer to file data start */
  ptr = mem;

  /* Read signature */
  Sign = *(DWORD *)ptr;
  ptr += 4;
  if (Sign != *(DWORD *)"G3DM")
  {
    free(mem);
    return FALSE;
  }

  /* Read sizes */
  NoofP = *(INT *)ptr;
  ptr += 4;
  NoofM = *(INT *)ptr;
  ptr += 4;
  NoofT = *(INT *)ptr;
  ptr += 4;

  /* Create primitives array */
  if (!AN6_RndPrimsCreate(Prs, NoofP))
  {
    free(mem);
    return FALSE;
  }

  /* Read all primitives */
  for (p = 0; p < NoofP; p++)
  {
    /* Read one primitive */
    INT NoofV, NoofI, MtlNo;
    an6VERTEX *V;
    INT *I;

    NoofV = *(INT *)ptr;
    ptr += 4;
    NoofI = *(INT *)ptr;
    ptr += 4;
    MtlNo = *(INT *)ptr;
    ptr += 4;

    V = (an6VERTEX *)ptr;
    ptr += sizeof(an6VERTEX) * NoofV;
    I = (INT *)ptr;
    ptr += sizeof(INT) * NoofI;

    /* Add a new primitive */
    AN6_RndPrimCreate(&Prs->Prims[p], AN6_RND_PRIM_TRIMESH,
      V, NoofV, I, NoofI);
    Prs->Prims[p].MtlNo = MtlNo;
  }

  /* Store first material number */
  first_mtl_no = AN6_RndMaterialsSize;

  /* Read all materials */
  for (m = 0; m < NoofM; m++)
  {
    /* Read one material */
    AN6_RndMtlAdd((an6MATERIAL *)ptr);
    ptr += sizeof(an6MATERIAL);
  }

  /* Update material numbers in primitives */
  for (p = 0; p < NoofP; p++)
    Prs->Prims[p].MtlNo += first_mtl_no;

  /* Store first texture number */
  first_tex_no = AN6_RndTexturesSize;

  /* Read all textures */
  for (t = 0; t < NoofT; t++)
  {
    /* Read one texture */
    an6TEXTURE *tex = (an6TEXTURE *)ptr;

    ptr += sizeof(an6TEXTURE);
    AN6_RndTexAddImg(tex->Name, tex->W, tex->H, (DWORD *)ptr);
    ptr += 4 * tex->W * tex->H;
  }

  /* Update texture numbers in materials */
  for (m = 0; m < NoofM; m++)
  {
    INT i;
    an6MATERIAL *mtl = &AN6_RndMaterials[first_mtl_no + m];
    
    for (i = 0; i < 8; i++)
      if (mtl->Tex[i] != -1)
        mtl->Tex[i] += first_tex_no;
  }

  free(mem);
  return TRUE;
} /* End of 'AN6_RndPrimsLoad' function */  

/* END OF 'RNDPRIMS.C' FILE */