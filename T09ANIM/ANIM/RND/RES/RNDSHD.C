#include <stdio.h>
#include <string.h>

#include "../../anim.h"

/* Load text from file function.
 * ARGUMENTS:
 *   - file name to be load from:
 *       CHAR *FileName;
 * RETURNS:
 *   (CHAR *) load text (allocated through malloc).
 */
static CHAR * AN6_RndShdLoadTextFromFile( CHAR *FileName )
{
  FILE *F;
  CHAR *txt;

  INT flen;

  if ((F = fopen(FileName, "r")) == NULL)
    return NULL;
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
  rewind(F);

  if ((txt = malloc(flen + 1)) == NULL)
  {
    fclose(F);
    return NULL;
  }
  memset(txt, 0, flen + 1);
  fread(txt, 1, flen, F);
  fclose(F);
  return txt;
} /* End of 'AN6_RndShdLoadTextFromFile' function */

/* Store log to file function.
 * ARGUMENTS:
 *   - message file prefix, shader name and text:
 *       CHAR *PartName, *Text;
 * RETURNS: None.
 */
static VOID AN6_RndShdLog( CHAR *Prefix, CHAR *Type, CHAR *Text )
{
  FILE *F;

  if ((F = fopen("BIN/SHADERS/DEFAULT", "a")) == NULL)
    return;
  fprintf(F, "%s/%s.GLSL:\n%s\n", Prefix, Type, Text);
  fclose(F);
} /* End of 'VG4_RndShdLog' function */

/* Load shader program from .GLSL files function.
 * ARGUMENTS:
 *   - shader file path to load (path\***.glsl):
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) load shader program Id.
 */
INT AN6_RndShdLoad( CHAR *ShaderFileNamePrefix )
{
  struct
  {
    CHAR *Name; /* Shader file suffix name */
    INT Type;   /* Shader OpenGL type */
    INT Id;     /* Shader Id created by OpenGL */
  } shd[] =
  {
    {"VERT", GL_VERTEX_SHADER},
    {"FRAG", GL_FRAGMENT_SHADER}
  };
  INT i, prg = 0, res, Ns = sizeof(shd) / sizeof(shd[0]);
  CHAR *txt;
  BOOL is_ok = TRUE;
  static CHAR Buf[3000];

  for (i = 0; is_ok && i < Ns; i++)
  {
    /* Load shader text from file */
    sprintf(Buf, "%s/%s.GLSL", ShaderFileNamePrefix, shd[i].Name);
    if ((txt = AN6_RndShdLoadTextFromFile(Buf)) == NULL)
    {
      AN6_RndShdLog(ShaderFileNamePrefix, shd[i].Name, "Error load file");
      is_ok = FALSE;
      break;
    }
    /* Create shader */
    if ((shd[i].Id = glCreateShader(shd[i].Type)) == 0)
    {
      AN6_RndShdLog(ShaderFileNamePrefix, shd[i].Name, "Error create shader");
      free(txt);
      is_ok = FALSE;
      break;
    }
    /* Attach shader source code to shader */
    glShaderSource(shd[i].Id, 1, &txt, NULL);
    free(txt);

    /* Compile shader */
    glCompileShader(shd[i].Id);
    glGetShaderiv(shd[i].Id, GL_COMPILE_STATUS, &res);
    if (res != 1)
    {
      glGetShaderInfoLog(shd[i].Id, sizeof(Buf), &res, Buf);
      AN6_RndShdLog(ShaderFileNamePrefix, shd[i].Name, Buf);
      is_ok = FALSE;
      break;
    }
  }

  if (is_ok)
  {
    if ((prg = glCreateProgram()) == 0)
    {
      is_ok = FALSE;
      AN6_RndShdLog(ShaderFileNamePrefix, "PROG", "Error create program");
    }
    else
    {
      /* Attach shaders to program */
      for (i = 0; i < Ns; i++)
        if (shd[i].Id != 0)
          glAttachShader(prg, shd[i].Id);
      /* Link shader program */
      glLinkProgram(prg);
      glGetProgramiv(prg, GL_LINK_STATUS, &res);
      if (res != 1)
      {
        glGetProgramInfoLog(prg, sizeof(Buf), &res, Buf);
        AN6_RndShdLog(ShaderFileNamePrefix, "PROG", Buf);
        is_ok = FALSE;
      }
    }
  }

  /* Error handle */
  if (!is_ok)
  {
    /* Remove all shaders */
    for (i = 0; i < Ns; i++)
      if (shd[i].Id != 0)
      {
        if (prg != 0)
          glDetachShader(prg, shd[i].Id);
        glDeleteShader(shd[i].Id);
      }
    /* Remove program */
    if (prg != 0)
      glDeleteProgram(prg);
    prg = 0;
  }

  return prg;
} /* End of 'VG4_RndShdLoad' function */


/* Unload shader program from memory files function.
 * ARGUMENTS:
 *   - shader program Id:
 *       INT ProgId;
 * RETURNS: None.
 */
VOID AN6_RndShdDelete( INT ProgId )
{
  INT shdrs[5], n, i;

  if (ProgId == 0)
    return;
  glGetAttachedShaders(ProgId, 5, &n, shdrs);
  for (i = 0; i < n; i++)
  {
    glDetachShader(ProgId, shdrs[i]);
    glDeleteShader(shdrs[i]);
  }
  glDeleteProgram(ProgId);
} /* End of 'AN6_RndShdDelete' function */

/***
 * Shader storage functions
 ***/

/* Array of shaders */
an6SHADER AN6_RndShaders[AN6_MAX_SHADERS];
/* Shadres array store size */
INT AN6_RndShadersSize;

/* Shader storage initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AN6_RndShdInit( VOID )
{
  AN6_RndShdAdd("BIN/SHADERS/DEFAULT");
} /* End of 'VG4_RndShdInit' function */

/* Shader storage deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AN6_RndShdClose( VOID )
{
  INT i;

  for (i = 0; i < AN6_RndShadersSize; i++)
    AN6_RndShdDelete(AN6_RndShaders[i].ProgId);
  AN6_RndShadersSize = 0;
} /* End of 'AN6_RndShdClose' function */

/* Add shader to stock from file function.
 * ARGUMENTS:
 *   - shader file path to load (path\***.glsl):
 *       CHAR *ShaderFileNamePrefix;
 * RETURNS:
 *   (INT) new shader stock number.
 */
INT AN6_RndShdAdd( CHAR *ShaderFileNamePrefix )
{
  if (AN6_RndShadersSize >= AN6_MAX_SHADERS)
    return 0;

  strncpy(AN6_RndShaders[AN6_RndShadersSize].Name,
    ShaderFileNamePrefix, AN6_STR_MAX - 1);
  AN6_RndShaders[AN6_RndShadersSize].ProgId =
    AN6_RndShdLoad(ShaderFileNamePrefix);

  return AN6_RndShadersSize++;
} /* End of 'AN6_RndShdAdd' function */

/* Update from file all load shaders function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID AN6_RndShdUpdate( VOID )
{
  INT i;

  for (i = 0; i < AN6_RndShadersSize; i++)
  {
    AN6_RndShdDelete(AN6_RndShaders[i].ProgId);
    AN6_RndShaders[i].ProgId = AN6_RndShdLoad(AN6_RndShaders[i].Name);
  }
} /* End of 'AN6_RndShdUpdate' function */

/* END OF 'RNDSHD.C' FILE */