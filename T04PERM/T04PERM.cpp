#include <stdio.h>
#include <windows.h>
#define max 10
INT p[max];

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
  INT i;

  if (pos == max)
    Store();
  else
  {
    Go(pos + 1);  
    for (i = pos + 1; i < max; i++)
    {
      Swap(&p[pos], &p[i]);
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