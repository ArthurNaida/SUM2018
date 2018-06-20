#include <windows.h>
#include <time.h>
#include "anim.h"
#include "../units.h"

typedef struct
{
  UNIT_BASE_FIELDS;
  VEC Pos;
  an6PRIM Gr;
} an6UNIT_GROUND;

static VOID AN6_UnitInit( an6UNIT_GROUND *Uni, an6ANIM *Ani )
{
  an6GRID G;
  an6MATERIAL Mtl =
  {
    "Ground material",
    {0.2, 0.2, 0.2}, {0.8, 0.8, 0.8}, {0.3, 0.3, 0.3}, 24,
    1,
    {-1, -1, -1, -1, -1, -1, -1, -1},
    "",
    0
  };
} 

static VOID AN6_UnitClose( an6UNIT_GROUND *Uni, an6ANIM *Ani )
{
  AN6_RndPrimsFree(&Uni->GROUND);
} 

static VOID AN6_UnitResponse( an6UNIT_GROUND *Uni, an6ANIM *Ani )
{
  //Uni->Pos = PointTransform(Uni->Pos, MatrRotateY(AN6_Anim.GlobalTime));
}

static VOID AN6_UnitRender( an6UNIT_GROUND *Uni, an6ANIM *Ani )
{
  
  AN6_RndPrimsDraw(&Uni->GROUND, MatrMulMatr(MatrTranslate(Uni->Pos), MatrScale(VecSet(10, 10, 10))));
}

an6UNIT * AN6_UnitCreateGROUND( VOID )
{
  an6UNIT_GROUND *Uni;

  if ((Uni = (an6UNIT_GROUND *)AN6_AnimUnitCreate(sizeof(an6UNIT_GROUND))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)AN6_UnitInit;
  Uni->Response = (VOID *)AN6_UnitResponse;
  Uni->Render = (VOID *)AN6_UnitRender;
  return (an6UNIT *)Uni;
}                         