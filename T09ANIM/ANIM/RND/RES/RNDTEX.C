#include <stdio.h>
#include <string.h>

#include "../../anim.h"

/* Array of textures */
an6TEXTURE AN6_RndTextures[AN6_MAX_TEXTURES];
/* Shadres array store size */
INT AN6_RndTexturesSize;

/* Texture storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AN6_RndTexInit( VOID )
{
  AN6_RndTexturesSize = 0;
} /* End of 'AN6_RndTexInit' function */

/* Texture storage deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AN6_RndTexClose( VOID )
{
  INT i;

  for (i = 0; i < AN6_RndTexturesSize; i++)
    glDeleteTextures(1, &AN6_RndTextures[i].TexId);
  AN6_RndTexturesSize = 0;
} /* End of 'AN6_RndTexClose' function */

/* Add texture from image to stock from file function.
 * ARGUMENTS:
 *   - image name:
 *       CHAR *Name;
 *   - image size:
 *       INT W, H;
 *   - image pixels data:
 *       DWORD *Bits;
 * RETURNS:
 *   (INT) new texture stock number.
 */
INT AN6_RndTexAddImg( CHAR *Name, INT W, INT H, DWORD *Bits )
{
  if (AN6_RndTexturesSize >= AN6_MAX_TEXTURES)
    return -1;
  /* Allocate texture space */
  glGenTextures(1, &AN6_RndTextures[AN6_RndTexturesSize].TexId);
  glBindTexture(GL_TEXTURE_2D, AN6_RndTextures[AN6_RndTexturesSize].TexId);

  /* Upload texture */
  gluBuild2DMipmaps(GL_TEXTURE_2D, 4, W, H, GL_BGRA_EXT, GL_UNSIGNED_BYTE, Bits);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  strncpy(AN6_RndTextures[AN6_RndTexturesSize].Name, Name, AN6_STR_MAX - 1);
  AN6_RndTextures[AN6_RndTexturesSize].W = W;
  AN6_RndTextures[AN6_RndTexturesSize].H = H;

  glBindTexture(GL_TEXTURE_2D, 0);

  return AN6_RndTexturesSize++;
} /* End of 'AN6_RndTexAddImg' function */

/* Add texture to stock from file function.
 * ARGUMENTS:
 *   - texture file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (INT) new texture stock number (-1 if error occured).
 */
INT AN6_RndTexAdd( CHAR *FileName )
{
  INT n;
  an6IMAGE Img;

  if (!AN6_RndImgLoad(&Img, FileName))
    return -1;
  n = AN6_RndTexAddImg(FileName, Img.W, Img.H, Img.Bits);
  AN6_RndImgFree(&Img);
  return n;
} /* End of 'AN6_RndTexAdd' function */

/* END OF 'RNDTEX.C' FILE */