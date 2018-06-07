#include <stdio.h>
#include "VEC.H"

#define PI 3.14159265358979323846
#define D2R(A) ((A) * (PI / 180))
typedef DOUBLE DBL;
typedef struct TagMatr
{
  DBL M[4][4];
} MATR;

VEC VecSet( INT x, INT y, INT z)
{
  VEC v;
  v.x = x;
  v.y = y;
  v.z = z;
  return v;
}

VEC VecAddVec(VEC v1, VEC v2)
{
  v.x = v1.x + v2.x;
  v.y = v1.y + v2.y;
  v.z = v1.z + v2.z;

  return v;
}

VEC VecSubVec(VEC v1, VEC v2)
{
  v.x = v1.x - v2.x;
  v.y = v1.y - v2.y;
  v.z = v1.z - v2.z;

  return v;
}

VEC VecMulNum( VEC v1, INT i )
{
  v.x = v1.x * i;
  v.y = v1.y * i;
  v.z = v1.z * i;

  return v;
}

VEC VecDivNum( VEC v1, INT i )
{
  v.x = v1.x / i;
  v.y = v1.y / i;
  v.z = v1.z / i;

  return v;
}

VEC VecDotVec( VEC v1, VEC v2 )
{
  v.x = v1.x * v2.x;
  v.y = v1.y * v2.y;
  v.z = v1.z * v2.z;

  return v;
}

VEC VecCrossVec( VEC v1, VEC v2 )
{
  v.x = v1.y * v2.z - v1.z * v2.y;
  v.y = v1.z * v2.x - v1.x * v2.z;
  v.z = v1.x * v2.y - v1.y * v2.x;

  return v;
}

VEC VecLen( VEC v1 )
{
  

  return
}


