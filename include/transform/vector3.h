#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>

struct Vector3 {
  double x;
  double y;
  double z;
};

struct Vector3 cgVector3Zero() {
  struct Vector3 vectorZero = {0, 0, 0};
  return vectorZero;
}

struct Vector3 cgVector3Negative(struct Vector3 const * const vector) {
  struct Vector3 vectorMinus = {
    .x = -vector->x,
    .y = -vector->y,
    .z = -vector->z
  };
  return vectorMinus;
}

struct Vector3 cgVector3Add(struct Vector3 const * const vector1, struct Vector3 const * const vector2) {
  struct Vector3 vector3 = {
    .x = vector1->x + vector2->x,
    .y = vector1->y + vector2->y,
    .z = vector1->z + vector2->z
  };
  return vector3;
}

struct Vector3 cgVector3ScalarProduct(double const scalar, struct Vector3 const * const vector) {
  struct Vector3 vectorScale = {
    .x = scalar * vector->x,
    .y = scalar * vector->y,
    .z = scalar * vector->z
  };
  return vectorScale;
}

double cgVector3DotProduct(struct Vector3 const * const vector1, struct Vector3 const * const vector2) {
  return vector1->x * vector2->x + vector1->y * vector2->y + vector1->z * vector2->z;
}

struct Vector3 cgVector3CrossProduct(struct Vector3 const * const vector1, struct Vector3 const * const vector2) {
  struct Vector3 vector3 = {
    .x = vector1->y * vector2->z - vector1->z * vector2->y,
    .y = vector1->z * vector2->x - vector1->x * vector2->z,
    .z = vector1->x * vector2->y - vector1->y * vector2->x
  };
  return vector3;
}

struct Vector3 cgVector3Normalize(struct Vector3 const * const vector) {
  const double mag = sqrt(cgVector3DotProduct(vector, vector));
  struct Vector3 vectorNormal = {
    .x = vector->x/mag,
    .y = vector->y/mag,
    .z = vector->z/mag
  };
  return vectorNormal;
}

#endif
