#include <stdio.h>
#include <windows.h>
#define n 3

INT i, j, k, p;
DOUBLE A[n][n];

DOUBLE Detg( VOID )
{
  DOUBLE coef, det = 1;

  for (j = 0; j < n; j++)
  {
    for (k = i + 1; k < n; k++)
    {
      coef = A[k][i] / A[i][i];
      for (p = i; p < n; p++)
        A[i][p] -= A[i][p] * coef;
    }
  }

  for (i = 0; i < n; i++)
  {
    det *= A[i][i];
  }
  
  return det;
}

VOID LongMatrix( VOID )
{
  FILE *F;
  F = fopen("DET.TXT", "a");

  if (F == NULL)          
    return;                

  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      fscanf(F, "%lf", &A[i][j]);
    }
  }

  fprintf(F, "\n");
  fprintf(F, "%lf", Detg());
  fclose(F);  
}

VOID main( VOID )
{
  LongMatrix();
}