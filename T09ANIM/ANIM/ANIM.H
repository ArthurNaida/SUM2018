#ifndef __ANIM_H_
#define __ANIM_H_

#include "rnd\rnd.h"

#define AN6_MAX_UNITS 3000

#define UNIT_BASE_FIELDS \
  VOID (*Init)( an6UNIT *Uni, an6ANIM *Ani );      \
  VOID (*Close)( an6UNIT *Uni, an6ANIM *Ani );     \
  VOID (*Response)( an6UNIT *Uni, an6ANIM *Ani );  \
  VOID (*Render)( an6UNIT *Uni, an6ANIM *Ani )

typedef struct tagan6UNIT an6UNIT;

typedef struct tagan6ANIM
{
  HWND hWnd;
  HDC hDC;
  INT W, H;
  an6UNIT *Units[AN6_MAX_UNITS];
  INT NumOfUnits;
  HBITMAP hBm;
  BYTE Keys[256];
  BYTE KeysOld[256];
  BYTE KeysClick[256];
  INT Mx, My, Mz, Mdx, Mdy, Mdz;
  DBL GlobalTime; 
  DBL GlobalDeltaTime; /* Global time and interframe interval */
  DBL Time;
  DBL DeltaTime;             /* Time with pause and interframe interval */
  DBL FPS;   /* Frames per second value */
  BOOL IsPause;   /* Pause flag */

} an6ANIM;

struct tagan6UNIT
{
  UNIT_BASE_FIELDS;
};

extern an6ANIM AN6_Anim;

VOID AN6_AnimInit( HWND hWnd );
VOID AN6_AnimClose( VOID );
VOID AN6_AnimRender( VOID );
VOID AN6_AnimResize( INT w, INT h );
VOID AN6_AnimCopyFrame( HDC hDC );
VOID AN6_AnimUnitAdd( an6UNIT *Uni );
VOID AN6_AnimKeyboardResponse( VOID );
VOID AN6_AnimMouseResponse( VOID );
VOID AN6_TimerInit( VOID );
VOID AN6_TimerResponse( VOID );

an6UNIT * AN6_AnimUnitCreate( INT Size );

#endif 