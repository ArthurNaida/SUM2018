#include "anim.h"

an6ANIM AN6_Anim;

VOID AN6_AnimInit( HWND hWnd )
{
  AN6_RndInit(hWnd);
  AN6_TimerInit();
}

VOID AN6_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < AN6_Anim.NumOfUnits; i++)
  {
    AN6_Anim.Units[i]->Close(AN6_Anim.Units[i], &AN6_Anim);
    free(AN6_Anim.Units[i]);
  }
  AN6_Anim.NumOfUnits = 0;
  AN6_RndClose();
}

VOID AN6_AnimRender( VOID )
{
  INT i;

  AN6_AnimKeyboardResponse();
  AN6_AnimMouseResponse();

  for (i = 0; i < AN6_Anim.NumOfUnits; i++)
    AN6_Anim.Units[i]->Response(AN6_Anim.Units[i], &AN6_Anim);

  AN6_RndStart();
  AN6_TimerResponse();

  for (i = 0; i < AN6_Anim.NumOfUnits; i++)
    AN6_Anim.Units[i]->Render(AN6_Anim.Units[i], &AN6_Anim);
  AN6_RndEnd();
}

VOID AN6_AnimResize( INT w, INT h )
{
  AN6_RndResize(w, h);
}

VOID AN6_AnimCopyFrame( HDC hDC )
{
  AN6_RndCopyFrame(hDC);
}

VOID AN6_AnimUnitAdd( an6UNIT *Uni )
{
  if (AN6_Anim.NumOfUnits < AN6_MAX_UNITS)
    AN6_Anim.Units[AN6_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &AN6_Anim);
}