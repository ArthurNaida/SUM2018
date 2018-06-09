#include <stdio.h>
#include <windows.h>
#define max 3
INT p[max];
BOOL IsParity, spr, x;

VOID Swap( INT *a, INT *b )
{
  INT tmp = *a;

  *a = *b;
  *b = tmp;
}
VOID Store( VOID )
{
  INT i;
  FILE *F;
  F = fopen("PERM.TXT", "a");

  if (F == NULL)                                                     
    return;

  for (i = 0; i < max; i++)
    fprintf(F, "%i", p[i]);
  
  fprintf(F, "\n"); 
  fclose(F);
}
VOID Go( INT pos )
{
  INT i = 0;

  if (pos == max)
  {
    Store();
  }
  else
  { 
    Go(pos + 1);

    for (i = pos + 1; i < max; i++)
    {
      Swap(&p[pos], &p[i]);
      IsParity = !IsParity;
      IsParity = spr;     
      Go(pos + 1);       
    }
  }
}

VOID main( VOID )
{
  INT i;

  for (i = 0; i < max; i++)
    p[i] = i + 1;
  Go(0);
}