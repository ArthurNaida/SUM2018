#include "anim.h"

typedef struct tagan6UNIT_CONTROL
{
  UNIT_BASE_FIELDS;
  VEC CamLoc;
  VEC CamDir;
  DBL Speed;
} an6UNIT_CONTROL;

static VOID AN6_UnitInit( an6UNIT_CONTROL *Uni, an6ANIM *Ani )
{
  Uni->CamLoc =
    VecAddVec(Uni->CamLoc,
      VecMulNum(Uni->CamDir, Ani->DeltaTime * Uni->Speed *
        (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN])));

} 

static VOID AN6_UnitClose( an6UNIT_CONTROL *Uni, an6ANIM *Ani )
{
} 

static VOID AN6_UnitResponse( an6UNIT_CONTROL *Uni, an6ANIM *Ani )
{
  if (Ani->KeysClick[VK_LBUTTON])
    Ani->IsPause = !Ani->IsPause;
    
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