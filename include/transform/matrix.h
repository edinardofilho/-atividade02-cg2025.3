#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include "vector3.h"

#define PI 3.14159265359

struct Matrix4 {
  double matrix[4][4];
};

struct Matrix4 cgMatrixZero() {
  struct Matrix4 matrix = {
    .matrix = {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    }
  };
  return matrix;
}

struct Matrix4 cgMatrixIdentity() {
  struct Matrix4 matrix = {
    .matrix = {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };
  return matrix;
}

struct Matrix4 cgMatrixUniformScale(double const scale) {
  struct Matrix4 matrix = {
    .matrix = {
      {scale, 0, 0, 0},
      {0, scale, 0, 0},
      {0, 0, scale, 0},
      {0, 0, 0    , 1}
    }
  };
  return matrix;
}

struct Matrix4 cgMatrixVector3Scale(struct Vector3 const * const scaleVector) {
  struct Matrix4 matrix = {
    .matrix = {
      {scaleVector->x, 0             , 0             , 0},
      {0             , scaleVector->y, 0             , 0},
      {0             , 0             , scaleVector->z, 0},
      {0             , 0             , 0             , 1}
    }
  };
  return matrix;
}

struct Matrix4 cgMatrixTranslation(struct Vector3 const * const vector) {
  struct Matrix4 matrix = {
    .matrix = {
      {1, 0, 0, vector->x},
      {0, 1, 0, vector->y},
      {0, 0, 1, vector->z},
      {0, 0, 0, 1        }
    }
  };
  return matrix;
}

struct Matrix4 cgMatrixRotation(struct Vector3 const * const rotationAxis, double const rotationDegree) {
  const struct Vector3 vec = cgVector3Normalize(rotationAxis);
  const double rotationRadian = rotationDegree * PI / 180.0;
  const double cossine = cos(rotationRadian);
  const double sine = sin(rotationRadian);
  
  struct Matrix4 matrix = {
    .matrix = {
      {vec.x*vec.x*(1 - cossine) + cossine,    vec.x*vec.y*(1 - cossine) - vec.z*sine, vec.x*vec.z*(1 - cossine) + vec.y*sine, 0},
      {vec.x*vec.y*(1 - cossine) + vec.z*sine, vec.y*vec.y*(1 - cossine) + cossine   , vec.y*vec.z*(1 - cossine) - vec.x*sine, 0},
      {vec.x*vec.z*(1 - cossine) - vec.y*sine, vec.z*vec.y*(1 - cossine) + vec.x*sine, vec.z*vec.z*(1 - cossine) + cossine   , 0},
      {0                                     , 0                                     , 0                                     , 1}
    }
  };
  return matrix;
}

struct Matrix4 cgMatrixPerspective(double const fovDegree, double const aspectRatio, double const nearPlane, double const farPlane) {
  const double fovRadian = fovDegree * PI / 180.0;
  const double fovY = 1.0/tan(fovDegree/2.0);
  const double fovX = fovY/aspectRatio;
  const double B = -2*farPlane*nearPlane/(farPlane-nearPlane);
  const double A = -(farPlane+nearPlane)/(farPlane-nearPlane);

  struct Matrix4 matrix = {
    .matrix = {
      {fovX, 0   ,  0, 0},
      {0   , fovY,  0, 0},
      {0   , 0   ,  A, B},
      {0   , 0   , -1, 0}
    }
  };
  return matrix;
}

struct Matrix4 cgMatrixMatrixMultiplication(struct Matrix4 const * const matrixLeft, struct Matrix4 const * const matrixRight) {
  struct Matrix4 matrix = cgMatrixZero();

  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      for (int k = 0; k < 4; k++)
        matrix.matrix[i][j] += matrixLeft->matrix[i][k] * matrixRight->matrix[k][j];

  return matrix;
}

void cgMatrixFlatten(struct Matrix4 const * const m, float * mArray) {
  double const * const elements = &(m)->matrix[0][0];
  for (int i = 0; i < 16; i++)
    mArray[i] = (float)elements[i];
}

#endif
