#include <windows.h>
#include <time.h>
#include "anim.h"
#include "../units.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  an6PRIMS HELICOPTER;
} an6UNIT_HELICOPTER;

static VOID AN6_UnitInit( an6UNIT_HELICOPTER *Uni, an6ANIM *Ani )
{
  AN6_RndPrimsLoad(&Uni->HELICOPTER, "BIN/MODELS/WA-HELI.G3DM");
  Uni->Pos = VecSet(-0.06, 0, 1.22);
} 

static VOID AN6_UnitClose( an6UNIT_HELICOPTER *Uni, an6ANIM *Ani )
{
  AN6_RndPrimsFree(&Uni->HELICOPTER);
} 

static VOID AN6_UnitResponse( an6UNIT_HELICOPTER *Uni, an6ANIM *Ani )
{
  //Uni->Pos = PointTransform(Uni->Pos, MatrRotateY(AN6_Anim.GlobalTime));
}

static VOID AN6_UnitRender( an6UNIT_HELICOPTER *Uni, an6ANIM *Ani )
{
  
  AN6_RndPrimsDraw(&Uni->HELICOPTER, MatrMulMatr(MatrTranslate(Uni->Pos), MatrScale(VecSet(10, 10, 10))));
}

an6UNIT * AN6_UnitCreateHELICOPTER( VOID )
{
  an6UNIT_HELICOPTER *Uni;

  if ((Uni = (an6UNIT_HELICOPTER *)AN6_AnimUnitCreate(sizeof(an6UNIT_HELICOPTER))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)AN6_UnitInit;
  Uni->Response = (VOID *)AN6_UnitResponse;
  Uni->Render = (VOID *)AN6_UnitRender;
  return (an6UNIT *)Uni;
} 