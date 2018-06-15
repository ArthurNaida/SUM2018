#include "anim.h"

typedef struct tagan6UNIT_CONTROL
{
  UNIT_BASE_FIELDS;
  VEC CamLoc;
  VEC CamDir;
  DBL Speed;
} an6UNIT_CONTROL;

static VOID AN6_UnitInit2( an6UNIT_CONTROL *Uni, an6ANIM *Ani )
{

} 

static VOID AN6_UnitClose2( an6UNIT_CONTROL *Uni, an6ANIM *Ani )
{
}

static VOID AN6_UnitResponse2( an6UNIT_CONTROL *Uni, an6ANIM *Ani )
{
  if (Ani->Keys[VK_SHIFT] && Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;
  if (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN] > 0)
    Uni->CamLoc.y += Ani->Time;
  else if (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN] < 0)
    Uni->CamLoc.y -= Ani->Time;

  

  AN6_RndCamSet(VecSet(60, 25, Uni->CamLoc.y + 15), VecSet(0, 0, 0), VecSet(0, 1, 0));
} 

static VOID AN6_UnitRender2( an6UNIT_CONTROL *Uni, an6ANIM *Ani )
{
  static CHAR Buf[1000];

  sprintf(Buf, "FPS: %.3f", Ani->FPS);
  SetWindowText(Ani->hWnd, Buf);
}

an6UNIT * AN6_UnitCreateCONTROL( VOID )
{
  an6UNIT_CONTROL *Uni;

  if ((Uni = (an6UNIT_CONTROL *)AN6_AnimUnitCreate(sizeof(an6UNIT_CONTROL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)AN6_UnitInit2;
  Uni->Response = (VOID *)AN6_UnitResponse2;
  Uni->Render = (VOID *)AN6_UnitRender2;
  return (an6UNIT *)Uni;
} 