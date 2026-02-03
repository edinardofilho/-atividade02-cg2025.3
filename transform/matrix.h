#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include "vector3.h"
#include "vector4.h"

const double PI = 3.14159265359;

struct Matrix4 {
  double matrix[4][4];
};

struct Matrix4 cgMatrixZero() {
  struct Matrix4 mat = {
    .matrix = {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    }
  };
  return mat;
}

struct Matrix4 cgMatrixIdentity() {
  struct Matrix4 mat = {
    .matrix = {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };
  return mat;
}

struct Matrix4 cgMatrixScale(const double scale) {
  struct Matrix4 mat = {
    .matrix = {
      {scale, 0, 0, 0},
      {0, scale, 0, 0},
      {0, 0, scale, 0},
      {0, 0, 0    , 1}
    }
  };
  return mat;
}

struct Matrix4 cgMatrixTranslation(const double x, const double y, const double z) {
  struct Matrix4 mat = {
    .matrix = {
      {1, 0, 0, x},
      {0, 1, 0, y},
      {0, 0, 1, z},
      {0, 0, 0, 1}
    }
  };
  return mat;
}

struct Matrix4 cgMatrixRotatation(const struct Vector3 rotationAxis, const double degree) {
  const struct Vector3 vec = cgVector3Normalize(rotationAxis);
  const double rad = degree * PI / 180.0f;
  const double cossine = cos(rad);
  const double sine = sin(rad);
  
  struct Matrix4 mat = {
    .matrix = {
      {vec.x*vec.x*(1 - cossine) + cossine,    vec.x*vec.y*(1 - cossine) - vec.z*sine, vec.x*vec.z*(1 - cossine) + vec.y*sine, 0},
      {vec.x*vec.y*(1 - cossine) + vec.z*sine, vec.y*vec.y*(1 - cossine) + cossine   , vec.y*vec.z*(1 - cossine) - vec.x*sine, 0},
      {vec.x*vec.z*(1 - cossine) - vec.y*sine, vec.z*vec.y*(1 - cossine) + vec.x*sine, vec.z*vec.z*(1 - cossine) + cossine   , 0},
      {0                                     , 0                                     , 0                                     , 1}
    }
  };
  return mat;
}

struct Matrix4 cgMatrixMatrixMultiplication(const struct Matrix4 m1, const struct Matrix4 m2) {
  struct Matrix4 mat = cgMatrixZero();

  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      for (int k = 0; k < 4; k++)
        mat.matrix[i][j] += m1.matrix[i][k] * m2.matrix[k][j];

  return mat;
}

struct Vector4 cgMatrixVector4Multiplication(const struct Matrix4 m, const struct Vector4 v) {
  struct Vector4 v_ret = {
    .x = v.x * m.matrix[0][0] + v.y * m.matrix[0][1] + v.z * m.matrix[0][2] + v.w * m.matrix[0][3],
    .y = v.x * m.matrix[1][0] + v.y * m.matrix[1][1] + v.z * m.matrix[1][2] + v.w * m.matrix[1][3],
    .z = v.x * m.matrix[2][0] + v.y * m.matrix[2][1] + v.z * m.matrix[2][2] + v.w * m.matrix[2][3],
    .w = v.x * m.matrix[3][0] + v.y * m.matrix[3][1] + v.z * m.matrix[3][2] + v.w * m.matrix[3][3]
  };
  return v_ret;
}

#endif
