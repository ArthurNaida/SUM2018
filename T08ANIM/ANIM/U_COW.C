#include <windows.h>
#include <time.h>
#include "anim.h"
#include "../units.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  an6PRIM COW;
} an6UNIT_COW;

static VOID AN6_UnitInit( an6UNIT_COW *Uni, an6ANIM *Ani )
{
  AN6_RndPrimLoad(&Uni->COW, "cow.object");
  Uni->Pos = VecSet(2, 0, 2);
} 

static VOID AN6_UnitClose( an6UNIT_COW *Uni, an6ANIM *Ani )
{
  AN6_RndPrimFree(&Uni->COW);
} 

static VOID AN6_UnitResponse( an6UNIT_COW *Uni, an6ANIM *Ani )
{
} 

static VOID AN6_UnitRender( an6UNIT_COW *Uni, an6ANIM *Ani )
{
  AN6_RndPrimDraw(&Uni->COW, MatrMulMatr(MatrTranslate(VecSet(0, sin(Ani->Time) * 15, 0)), MatrRotateY(Ani->Time * 200)));
}

an6UNIT * AN6_UnitCreateCOW( VOID )
{
  an6UNIT_COW *Uni;

  if ((Uni = (an6UNIT_COW *)AN6_AnimUnitCreate(sizeof(an6UNIT_COW))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)AN6_UnitInit;
  Uni->Response = (VOID *)AN6_UnitResponse;
  Uni->Render = (VOID *)AN6_UnitRender;
  return (an6UNIT *)Uni;
} 