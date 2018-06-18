#include "anim.h"

typedef struct tagan6UNIT_CONTROL
{
  UNIT_BASE_FIELDS;
  VEC CamLoc;
  VEC CamAt;
  VEC CamDir;
  FLT Speed;
} an6UNIT_CONTROL;

static VOID AN6_UnitInit( an6UNIT_CONTROL *Uni, an6ANIM *Ani )
{
  Uni->CamLoc = VecSet(1, 10, -30);
  Uni->CamAt = VecSet(0, 10, 0);
  Uni->CamDir = VecSet(0, 1, 0);
} 

static VOID AN6_UnitClose( an6UNIT_CONTROL *Uni, an6ANIM *Ani )
{
} 

static VOID AN6_UnitResponse( an6UNIT_CONTROL *Uni, an6ANIM *Ani )
{
  if (Ani->Keys[VK_SHIFT] && Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  if (Ani->Keys[VK_LBUTTON])
  {
    Uni->CamLoc =
      PointTransform(Uni->CamLoc, MatrRotateY(-30 * Ani->GlobalDeltaTime * AN6_Anim.Mdx));
    Uni->CamAt =
      PointTransform(Uni->CamAt, MatrRotateY(-30 * Ani->GlobalDeltaTime * AN6_Anim.Mdx));
    Uni->CamLoc =
      PointTransform(Uni->CamLoc, MatrRotateX(-30 * Ani->GlobalDeltaTime * AN6_Anim.Mdy));
  }
  AN6_RndCamSet(Uni->CamLoc, Uni->CamAt, Uni->CamDir);
  
    
}

static VOID AN6_UnitRender( an6UNIT_CONTROL *Uni, an6ANIM *Ani )
{
  static CHAR Buf[1000];

  sprintf(Buf, "FPS: %.3f", AN6_Anim.FPS);
  SetWindowText(AN6_Anim.hWnd, Buf);
}

an6UNIT * AN6_UnitCreateCONTROL( VOID )
{
  an6UNIT_CONTROL *Uni;

  if ((Uni = (an6UNIT_CONTROL *)AN6_AnimUnitCreate(sizeof(an6UNIT_CONTROL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)AN6_UnitInit;
  Uni->Response = (VOID *)AN6_UnitResponse;
  Uni->Render = (VOID *)AN6_UnitRender;
  return (an6UNIT *)Uni;
} 