#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include "vector3.h"

#define PI 3.14159265359

typedef struct {
  double matrix[4][4];
} Matrix4;

Matrix4 cgMatrixZero() {
  Matrix4 matrix = {
    .matrix = {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0}
    }
  };
  return matrix;
}

Matrix4 cgMatrixIdentity() {
  Matrix4 matrix = {
    .matrix = {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };
  return matrix;
}

Matrix4 cgMatrixUniformScale(double const scale) {
  Matrix4 matrix = {
    .matrix = {
      {scale, 0, 0, 0},
      {0, scale, 0, 0},
      {0, 0, scale, 0},
      {0, 0, 0    , 1}
    }
  };
  return matrix;
}

Matrix4 cgMatrixScaleVector3(Vector3 const * const scaleVector) {
  Matrix4 matrix = {
    .matrix = {
      {scaleVector->x, 0             , 0             , 0},
      {0             , scaleVector->y, 0             , 0},
      {0             , 0             , scaleVector->z, 0},
      {0             , 0             , 0             , 1}
    }
  };
  return matrix;
}

Matrix4 cgMatrixTranslation(Vector3 const * const vector) {
  Matrix4 matrix = {
    .matrix = {
      {1, 0, 0, vector->x},
      {0, 1, 0, vector->y},
      {0, 0, 1, vector->z},
      {0, 0, 0, 1        }
    }
  };
  return matrix;
}

Matrix4 cgMatrixRotation(Vector3 const * const rotationAxis, double const rotationDegree) {
  Vector3 const vec = cgVector3Normalize(rotationAxis);
  double const rotationRadian = rotationDegree * PI / 180.0;
  double const cossine = cos(rotationRadian);
  double const sine = sin(rotationRadian);
  
  Matrix4 matrix = {
    .matrix = {
      {vec.x*vec.x*(1 - cossine) + cossine,    vec.x*vec.y*(1 - cossine) - vec.z*sine, vec.x*vec.z*(1 - cossine) + vec.y*sine, 0},
      {vec.x*vec.y*(1 - cossine) + vec.z*sine, vec.y*vec.y*(1 - cossine) + cossine   , vec.y*vec.z*(1 - cossine) - vec.x*sine, 0},
      {vec.x*vec.z*(1 - cossine) - vec.y*sine, vec.z*vec.y*(1 - cossine) + vec.x*sine, vec.z*vec.z*(1 - cossine) + cossine   , 0},
      {0                                     , 0                                     , 0                                     , 1}
    }
  };
  return matrix;
}

Matrix4 cgMatrixPerspective(double const fovDegree, double const aspectRatio, double const nearPlane, double const farPlane) {
  double const fovRadian = fovDegree * PI / 180.0;
  double const fovY = 1.0/tan(fovDegree/2.0);
  double const fovX = fovY/aspectRatio;
  double const B = -2*farPlane*nearPlane/(farPlane-nearPlane);
  double const A = -(farPlane+nearPlane)/(farPlane-nearPlane);

  Matrix4 matrix = {
    .matrix = {
      {fovX, 0   ,  0, 0},
      {0   , fovY,  0, 0},
      {0   , 0   ,  A, B},
      {0   , 0   , -1, 0}
    }
  };
  return matrix;
}

Matrix4 cgMatrixMatrixMultiplication(Matrix4 const * const matrixLeft, Matrix4 const * const matrixRight) {
  Matrix4 matrix = cgMatrixZero();

  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      for (int k = 0; k < 4; k++)
        matrix.matrix[i][j] += matrixLeft->matrix[i][k] * matrixRight->matrix[k][j];

  return matrix;
}

void cgMatrixFlatten(Matrix4 const * const m, float * mArray) {
  double const * const elements = &(m)->matrix[0][0];
  for (int i = 0; i < 16; i++)
    mArray[i] = (float)elements[i];
}

#endif
