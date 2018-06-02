#include <windows.h>
#include <stdio.h>

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  INT l = 1, r = 100, m, ans;
  char Buf[200];

  sprintf(Buf, "Choose any number between\n%i and %i", l, r);
  MessageBox(NULL, Buf, "INFO",  MB_OK);
  while (l <= r)
  {
    m = (r + l) / 2; 
    sprintf(Buf, "Is your number more (YES), less (NO) or equal (CANCEL) than %i?", m);
    ans = MessageBox(NULL, Buf, "Game",  MB_YESNOCANCEL);
    if (ans == IDYES)
      l = m + 1;
    else if (ans == IDNO)
      r = m - 1;
    else
    {
      sprintf(Buf, "Your number is %i", m);
      MessageBox(NULL, Buf, "Win", MB_OK);
      break;
    }
  }
  return 0;
}