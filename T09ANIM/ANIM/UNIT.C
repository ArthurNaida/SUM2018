#include "anim.h"

static VOID AN6_UnitInit( an6UNIT *Uni, an6ANIM *Ani )
{
} 

static VOID AN6_UnitClose( an6UNIT *Uni, an6ANIM *Ani )
{
} 

static VOID AN6_UnitResponse( an6UNIT *Uni, an6ANIM *Ani )
{
} 

static VOID AN6_UnitRender( an6UNIT *Uni, an6ANIM *Ani )
{
}

an6UNIT * AN6_AnimUnitCreate( INT Size )
{
  an6UNIT *Uni;

  /* Memory allocation */
  if (Size < sizeof(an6UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);
  /* Setup unit methods */
  Uni->Init = AN6_UnitInit;
  Uni->Close = AN6_UnitClose;
  Uni->Response = AN6_UnitResponse;
  Uni->Render = AN6_UnitRender;
  return Uni;
} 

