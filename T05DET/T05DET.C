#include <stdio.h>
#include <windows.h>
#define max 10
DOUBLE A[max][max];
INT n;

VOID Swap( INT *a, INT *b )
{
  INT tmp = *a;

  *a = *b;
  *b = tmp
}

INT Go( INT pos )
{
  INT i;

  if (pos == max)
    return res;
  else
  {
    Go(pos + 1);  
    for (i = pos + 1; i < max; i++)
    {
      Swap(&p[pos], &p[i]);
      res += 
      Go(pos + 1);       
    }
  }
}

VOID Det( INT n, INT pr )
{
  INT i, j, sum;

  for (i = 0; i < n; i++)
  {
    sum += A[i][j];
  }
}

VOID LongMatrix( VOID )
{
  FILE *F;
  INT i, j;
  F = fopen("DET.TXT", "a");

  if (F == NULL)
    return;

  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      fscanf(F, "%lf", &A[i][j]);
    }
    fprintf(F, "\n");
  }
  fclose(F);
}

VOID main( VOID )
{

  LongMatrix();
}