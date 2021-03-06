#include <math.h>
#include <stdio.h>
#include <windows.h>

#define PI 3.14159265358979323846
#define D2R(A) ((A) * (PI / 180))
typedef DOUBLE DBL;

typedef struct
{
  DBL x, y, z;
} VEC;

typedef struct TagMatr
{
  DBL M[4][4];
} MATR;

__inline VEC VecSet( DBL x, DBL y, DBL z)
{
  VEC v;
  v.x = x;
  v.y = y;
  v.z = z;
  return v;
}

__inline VEC VecAddVec(VEC v1, VEC v2)
{
  VEC v;

  v.x = v1.x + v2.x;
  v.y = v1.y + v2.y;
  v.z = v1.z + v2.z;

  return v;
}

__inline VEC VecSubVec(VEC v1, VEC v2)
{
  VEC v;

  v.x = v1.x - v2.x;
  v.y = v1.y - v2.y;
  v.z = v1.z - v2.z;

  return v;
}

__inline VEC VecMulNum( VEC v1, INT i )
{
  VEC v;

  v.x = v1.x * i;
  v.y = v1.y * i;
  v.z = v1.z * i;

  return v;
}

__inline VEC VecDivNum( VEC v, DBL i )
{
  if (i != 0)
  {
    v.x = v.x / i;
    v.y = v.y / i;
    v.z = v.z / i;
  }
  return v;
}

__inline DBL VecDotVec( VEC v1, VEC v2 )
{
  DBL v;

  v = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

  return v;
}

__inline VEC VecCrossVec( VEC v1, VEC v2 )
{
  VEC v;

  v.x = v1.y * v2.z - v1.z * v2.y;
  v.y = v1.z * v2.x - v1.x * v2.z;
  v.z = v1.x * v2.y - v1.y * v2.x;

  return v;
}

__inline DBL VecLen( VEC v1 )
{
  DBL len = sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);

  return len;
}

__inline DBL VecLen2( VEC v1 )
{
  DBL len2 = v1.x * v1.x + v1.y * v1.y + v1.z * v1.z;

  return len2;
}

__inline VEC VecNormalize( VEC v1 )
{
  DBL len = VecLen(v1);

  if (len == 0 || len == 1)
    return v1;
  
  return VecDivNum(v1, len);
}

static MATR UnitMatrix =
{
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
  }
};

__inline MATR MatrIdentity( VOID )
{
  return UnitMatrix;
}

__inline MATR MatrTranslate( VEC T )
{
  MATR m = UnitMatrix;

  m.M[3][0] = T.x;
  m.M[3][1] = T.y;
  m.M[3][2] = T.z;

  return m;
}

__inline MATR MatrScale( VEC S )
{
  MATR M = UnitMatrix;

  M.M[0][0] = S.x;
  M.M[1][1] = S.y;
  M.M[2][2] = S.y;

  return M;
}
__inline MATR MatrRotateX( DBL AngleInDegree )
{
  DBL a = D2R(AngleInDegree), si = sin(a), co = cos(a);
  MATR M = 
  {
    {
      {1,   0,  0, 0},
      {0,  co, si, 0},
      {0, -si, co, 0},
      {0,   0,  0, 1}
    }
  };

  return M;
}

__inline MATR MatrRotateY( DBL AngleInDegree )
{
  DBL a = D2R(AngleInDegree), si = sin(a), co = cos(a);
  MATR M = 
  {
    {
      {co, 0, -si, 0},
      {0, 1, 0, 0},
      {si, 0, co, 0},
      {0, 0, 0, 1}
    }
  };

  return M;
}
__inline MATR MatrRotateZ( DBL AngleInDegree )
{
  DBL a = D2R(AngleInDegree), si = sin(a), co = cos(a);
  MATR M = 
  {
    {
      {co, si, 0, 0},
      {-si, co, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  return M;
}

__inline MATR MatrRotate( DBL AngleInDegree, VEC V )
{
  DBL A = D2R(AngleInDegree), si = sin(A), co = cos(A);
  MATR M =
  {
    {
      {co + V.x * V.x * (1 - co), V.x * V.y * (1 - co) + V.x * si, V.x * V.z * (1 - co) - V.y * si, 0},
      {V.y * V.x * (1 - co) - V.z * si, co + V.y * V.y * (1 - co), V.y * V.z * (1 - co) + V.x * si, 0},
      {V.z * V.x * (1 - co) + V.y * si, V.z * V.y * (1 - co) - V.x * si, co + V.z * V.z * (1 - co), 0},
      {0, 0, 0, 1}
    }
  };

  return M;
}

__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  MATR r;
  INT i, j, k;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (r.M[i][j] = 0, k = 0; k < 4; k++)
        r.M[i][j] += M1.M[i][k] * M2.M[k][j];

  return r;
}
__inline MATR MatrTranspose( MATR M )
{
  INT i, j;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
    {
      M.M[i][j] = M.M[j][i];
    }

  return M;
}


__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13, DBL A21, DBL A22, DBL A23, DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 - A11 * A23 * A32 - A12 * A21 * A33 + 
         A12 * A23 * A31 + A13 * A21 * A32 - A13 * A22 * A31;
}

__inline DBL MatrDeterm( MATR M )
{
  return
    M.M[0][0] * MatrDeterm3x3(M.M[1][1], M.M[1][2], M.M[1][3],
                              M.M[2][1], M.M[2][2], M.M[2][3],
                              M.M[3][1], M.M[3][2], M.M[3][3]) -
    M.M[0][1] * MatrDeterm3x3(M.M[1][0], M.M[1][2], M.M[1][3],
                              M.M[2][0], M.M[2][2], M.M[2][3],
                              M.M[3][0], M.M[3][2], M.M[3][3]) +
    M.M[0][2] * MatrDeterm3x3(M.M[1][0], M.M[1][1], M.M[1][3],
                              M.M[2][0], M.M[2][1], M.M[2][3],
                              M.M[3][0], M.M[3][1], M.M[3][3]) -
    M.M[0][3] * MatrDeterm3x3(M.M[1][0], M.M[1][1], M.M[1][2],
                              M.M[2][0], M.M[2][1], M.M[2][2],
                              M.M[3][0], M.M[3][1], M.M[3][2]);
}
__inline MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);

  if (det == 0)
    return UnitMatrix;

  r.M[0][0] =
    MatrDeterm3x3(M.M[1][1], M.M[1][2], M.M[1][3],
                  M.M[2][1], M.M[2][2], M.M[2][3],
                  M.M[3][1], M.M[3][2], M.M[3][3]);
  r.M[1][0] =
    -MatrDeterm3x3(M.M[1][0], M.M[1][2], M.M[1][3],
                   M.M[2][0], M.M[2][2], M.M[2][3],
                   M.M[3][0], M.M[3][2], M.M[3][3]);
  r.M[2][0] =
    MatrDeterm3x3(M.M[1][0], M.M[1][1], M.M[1][3],
                  M.M[2][0], M.M[2][1], M.M[2][3],
                  M.M[3][0], M.M[3][1], M.M[3][3]);
  r.M[3][0] =
    -MatrDeterm3x3(M.M[1][0], M.M[1][1], M.M[1][2],
                   M.M[2][0], M.M[2][1], M.M[2][2],
                   M.M[3][0], M.M[3][1], M.M[3][2]);

  r.M[0][1] =
    -MatrDeterm3x3(M.M[0][1], M.M[0][2], M.M[0][3],
                  M.M[2][1], M.M[2][2], M.M[2][3],
                  M.M[3][1], M.M[3][2], M.M[3][3]);
  r.M[1][1] =
    MatrDeterm3x3(M.M[0][0], M.M[0][2], M.M[0][3],
                  M.M[2][0], M.M[2][2], M.M[2][3],
                  M.M[3][0], M.M[3][2], M.M[3][3]);
  r.M[2][1] =
    -MatrDeterm3x3(M.M[0][0], M.M[0][1], M.M[0][3],
                   M.M[2][0], M.M[2][1], M.M[2][3],
                   M.M[3][0], M.M[3][1], M.M[3][3]);
  r.M[3][1] =
    MatrDeterm3x3(M.M[0][0], M.M[0][1], M.M[0][2],
                  M.M[2][0], M.M[2][1], M.M[2][2],
                  M.M[3][0], M.M[3][1], M.M[3][2]);

  r.M[0][2] =
    MatrDeterm3x3(M.M[0][1], M.M[0][2], M.M[0][3],
                 M.M[1][1], M.M[1][2], M.M[1][3],
                 M.M[3][1], M.M[3][2], M.M[3][3]);
  r.M[1][2] =
    -MatrDeterm3x3(M.M[0][0], M.M[0][2], M.M[0][3],
                   M.M[1][0], M.M[1][2], M.M[1][3],
                   M.M[3][0], M.M[3][2], M.M[3][3]);
  r.M[2][2] =
    MatrDeterm3x3(M.M[0][0], M.M[0][1], M.M[0][3],
                  M.M[1][0], M.M[1][1], M.M[1][3],
                  M.M[3][0], M.M[3][1], M.M[3][3]);
  r.M[3][2] =
    -MatrDeterm3x3(M.M[0][0], M.M[0][1], M.M[0][2],
                   M.M[1][0], M.M[1][1], M.M[1][2],
                   M.M[3][0], M.M[3][1], M.M[3][2]);

  r.M[0][3] =
    -MatrDeterm3x3(M.M[0][1], M.M[0][2], M.M[0][3],
                  M.M[1][1], M.M[1][2], M.M[1][3],
                  M.M[2][1], M.M[2][2], M.M[2][3]);
 
  r.M[1][3] =
    MatrDeterm3x3(M.M[0][0], M.M[0][2], M.M[0][3],
                  M.M[1][0], M.M[1][2], M.M[1][3],
                  M.M[2][0], M.M[2][2], M.M[2][3]);
  r.M[2][3] =
    -MatrDeterm3x3(M.M[0][0], M.M[0][1], M.M[0][3],
                   M.M[1][0], M.M[1][1], M.M[1][3],
                   M.M[2][0], M.M[2][1], M.M[2][3]);
  r.M[3][3] =
    MatrDeterm3x3(M.M[0][0], M.M[0][1], M.M[0][2],
                  M.M[1][0], M.M[1][1], M.M[1][2],
                  M.M[2][0], M.M[2][1], M.M[2][2]);

  r.M[0][0] /= det;
  r.M[1][0] /= det;
  r.M[2][0] /= det;
  r.M[3][0] /= det;
  r.M[0][1] /= det;
  r.M[1][1] /= det;
  r.M[2][1] /= det;
  r.M[3][1] /= det;
  r.M[0][2] /= det;
  r.M[1][2] /= det;
  r.M[2][2] /= det;
  r.M[3][2] /= det;
  r.M[0][3] /= det;
  r.M[1][3] /= det;
  r.M[2][3] /= det;
  r.M[3][3] /= det;

  return r;
} 

__inline MATR MatrView( VEC Loc, VEC At, VEC Up1)
{
  VEC Dir = VecNormalize(VecSubVec(At, Loc));
  VEC Right = VecNormalize(VecCrossVec(Dir, Up1));
  VEC Up = VecCrossVec(Right, Dir);
  MATR m = 
  {
    {
      {Right.x, Up.x, -Dir.x, 0},
      {Right.y, Up.y, -Dir.y, 0},
      {Right.z, Up.z, -Dir.z, 0},
      {-VecDotVec(Loc, Right), -VecDotVec(Loc, Up), VecDotVec(Loc, Dir), 1}
    }
  };

  return m;
}

__inline MATR MatrFrustum( DBL l, DBL r, DBL b, DBL t, DBL n, DBL f )
{
  MATR m =
  {
    {
      {2 * n / (r - l), 0, 0, 0},
      {0, 2 * n / (t - b), 0, 0},
      {(r + l)/(r - l), (t + b)/(t - b), -(f + n)/(f - n), -1},
      {0, 0, -2 * n * f / (f - n), 0}
    }
  };

  return m;
}

__inline VEC VecMulMatr4x4( VEC V, MATR M )
{
  DBL w = V.x * M.M[0][3] + V.y * M.M[1][3] + V.z * M.M[2][3] + M.M[3][3];

  return VecSet((V.x * M.M[0][0] + V.y * M.M[1][0] + V.z * M.M[2][0] + M.M[3][0]) / w,
                (V.x * M.M[0][1] + V.y * M.M[1][1] + V.z * M.M[2][1] + M.M[3][1]) / w,
                (V.x * M.M[0][2] + V.y * M.M[1][2] + V.z * M.M[2][2] + M.M[3][2]) / w);
}
