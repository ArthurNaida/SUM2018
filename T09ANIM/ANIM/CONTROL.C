#include "anim.h"

typedef struct tagan6UNIT_CONTROL
{
  UNIT_BASE_FIELDS;
  VEC CamLoc;
  VEC CamAt;
  VEC CamUp;
  FLT Speed;
} an6UNIT_CONTROL;

static VOID AN6_UnitInit( an6UNIT_CONTROL *Uni, an6ANIM *Ani )
{
  Uni->CamLoc = VecSet(1, 10, -30);
  Uni->CamAt = VecSet(0, 10, 0);
  Uni->CamUp = VecSet(0, 1, 0);
} 

static VOID AN6_UnitClose( an6UNIT_CONTROL *Uni, an6ANIM *Ani )
{
} 

static VOID AN6_UnitResponse( an6UNIT_CONTROL *Uni, an6ANIM *Ani )
{
  VEC Sub, Sub1;

  if (Ani->Keys[VK_SHIFT] && Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

   if (Ani->Keys[VK_HOME])
  {
    Uni->CamLoc = VecSet(100, 65, -113);
    Uni->CamAt = VecSet(0, 10, 0);
  }

  if (Ani->Keys[VK_LBUTTON])
  {
    Uni->CamAt =
      VecAddVec(Uni->CamLoc, VectorTransform(VecSubVec(Uni->CamAt, Uni->CamLoc),
        MatrRotate(0.2 * AN6_Anim.Mdy, VecNormalize(VecCrossVec(VecSubVec(Uni->CamAt, Uni->CamLoc), VecSet(0, 1, 0))))));
    Uni->CamAt =
      VecAddVec(Uni->CamLoc, VectorTransform(VecSubVec(Uni->CamAt, Uni->CamLoc), MatrRotateY(-0.2 * AN6_Anim.Mdx)));
  }

  Sub =
    VecMulNum(VecSubVec(Uni->CamAt, Uni->CamLoc), 0.2 * (Ani->Keys['W'] - Ani->Keys['S']));
  Sub1 =
    PointTransform(VecMulNum(VecSubVec(Uni->CamAt, Uni->CamLoc),
    -0.2 * (Ani->Keys['D'] - Ani->Keys['A'])), MatrRotateY(90));
  Uni->CamLoc =
    VecAddVec(VecAddVec(Uni->CamLoc, Sub), Sub1);
  Uni->CamAt = 
    VecAddVec(VecAddVec(Uni->CamAt, Sub), Sub1);

    AN6_RndCamSet(Uni->CamLoc, Uni->CamAt, Uni->CamUp);
}

static VOID AN6_UnitRender( an6UNIT_CONTROL *Uni, an6ANIM *Ani )
{
  static CHAR Buf[1000];

  sprintf(Buf, "FPS: %.3f, CamLocX: %.3f, CamLocY: %.3f, CamLocZ: %.3f, CamAtX: %.3f, CamAtY: %.3f, CamAtZ: %.3f",
    AN6_Anim.FPS, Uni->CamLoc.x, Uni->CamLoc.y, Uni->CamLoc.z, Uni->CamAt.x, Uni->CamAt.y, Uni->CamAt.z);
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