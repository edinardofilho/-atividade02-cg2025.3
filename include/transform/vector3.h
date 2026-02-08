#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>

typedef struct {
  double x;
  double y;
  double z;
} Vector3; 

Vector3 cgVector3Zero() {
  Vector3 vectorZero = {0, 0, 0};
  return vectorZero;
}

Vector3 cgVector3Negative(Vector3 const * const vector) {
  Vector3 vectorMinus = {
    .x = -vector->x,
    .y = -vector->y,
    .z = -vector->z
  };
  return vectorMinus;
}

Vector3 cgVector3Add(Vector3 const * const vector1, Vector3 const * const vector2) {
  Vector3 vector3 = {
    .x = vector1->x + vector2->x,
    .y = vector1->y + vector2->y,
    .z = vector1->z + vector2->z
  };
  return vector3;
}

Vector3 cgVector3ScalarProduct(double const scalar, Vector3 const * const vector) {
  Vector3 vectorScale = {
    .x = scalar * vector->x,
    .y = scalar * vector->y,
    .z = scalar * vector->z
  };
  return vectorScale;
}

double cgVector3DotProduct(Vector3 const * const vector1, Vector3 const * const vector2) {
  return vector1->x * vector2->x + vector1->y * vector2->y + vector1->z * vector2->z;
}

Vector3 cgVector3CrossProduct(Vector3 const * const vector1, Vector3 const * const vector2) {
  Vector3 vector3 = {
    .x = vector1->y * vector2->z - vector1->z * vector2->y,
    .y = vector1->z * vector2->x - vector1->x * vector2->z,
    .z = vector1->x * vector2->y - vector1->y * vector2->x
  };
  return vector3;
}

Vector3 cgVector3Normalize(Vector3 const * const vector) {
  const double mag = sqrt(cgVector3DotProduct(vector, vector));
  Vector3 vectorNormal = {
    .x = vector->x/mag,
    .y = vector->y/mag,
    .z = vector->z/mag
  };
  return vectorNormal;
}

#endif
