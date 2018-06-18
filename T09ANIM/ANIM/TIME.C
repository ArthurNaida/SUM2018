#include <time.h>
#include "../units.h"

static UINT64
    StartTime,    /* Start program time */
    OldTime,      /* Previous frame time */
    OldTimeFPS,   /* Old time FPS measurement */
    PauseTime,    /* Time during pause period */
    TimePerSec,   /* Timer resolution */
    FrameCounter; /* Frames counter */

VOID AN6_TimerInit( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  StartTime = OldTime = OldTimeFPS = t.QuadPart;
  FrameCounter = 0;
  AN6_Anim.IsPause = FALSE;
  AN6_Anim.FPS = 30.0;
  PauseTime = 0;
}

VOID AN6_TimerResponse( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceCounter(&t);
  /* Global time */
  AN6_Anim.GlobalTime = (FLT)(t.QuadPart - StartTime) / TimePerSec;
  AN6_Anim.GlobalDeltaTime = (FLT)(t.QuadPart - OldTime) / TimePerSec;

  /* Time with pause */
  if (AN6_Anim.IsPause)
  {
    AN6_Anim.DeltaTime = 0;
    PauseTime += t.QuadPart - OldTime;
  }
  else
  {
    AN6_Anim.DeltaTime = AN6_Anim.GlobalDeltaTime;
    AN6_Anim.Time = (FLT)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
  }
  /* FPS */
  FrameCounter++;
  if (t.QuadPart - OldTimeFPS > TimePerSec)
  {
    AN6_Anim.FPS = FrameCounter * TimePerSec / (FLT)(t.QuadPart - OldTimeFPS);
    OldTimeFPS = t.QuadPart;
    FrameCounter = 0;
  }
  OldTime = t.QuadPart;
}