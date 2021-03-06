#include "anim.h"

VOID AN6_AnimKeyboardResponse( VOID )
{
  INT i;

  GetKeyboardState(AN6_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    AN6_Anim.Keys[i] >>= 7;
    AN6_Anim.KeysClick[i] = AN6_Anim.Keys[i] && !AN6_Anim.KeysOld[i];
  }
  memcpy(AN6_Anim.KeysOld, AN6_Anim.Keys, 256);
}

VOID AN6_AnimMouseResponse( VOID )
{
  
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(AN6_Anim.hWnd, &pt);
  /* ���������� ��������� �� ������ */
  AN6_Anim.Mdx = pt.x - AN6_Anim.Mx;
  AN6_Anim.Mdy = pt.y - AN6_Anim.My;
  /* ���������� �������� */
  AN6_Anim.Mx = pt.x;
  AN6_Anim.My = pt.y;
}