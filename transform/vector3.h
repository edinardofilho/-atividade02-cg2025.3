#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>

struct Vector3 {
  double x;
  double y;
  double z;
};

struct Vector3 cgVector3Zero() {
  struct Vector3 v_zero = {0, 0, 0};
  return v_zero;
}

struct Vector3 cgVector3Negative(const struct Vector3 v) {
  struct Vector3 v_minus = {
    .x = -v.x,
    .y = -v.y,
    .z = -v.z
  };
  return v_minus;
}

struct Vector3 cgVector3Add(const struct Vector3 v1, const struct Vector3 v2) {
  struct Vector3 v3 = {
    .x = v1.x + v2.x,
    .y = v1.y + v2.y,
    .z = v1.z + v2.z
  };
  return v3;
}

double cgVector3DotProduct(const struct Vector3 v1, const struct Vector3 v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

struct Vector3 cgVector3CrossProduct(const struct Vector3 v1, const struct Vector3 v2) {
  struct Vector3 v3 = {
    .x = v1.y * v2.z - v1.z * v2.y,
    .y = v1.z * v2.x - v1.x * v2.z,
    .z = v1.x * v2.y - v1.y * v2.x
  };
  return v3;
}

struct Vector3 cgVector3Normalize(const struct Vector3 v) {
  const double mag = sqrt(cgVector3DotProduct(v, v));
  struct Vector3 v_normal = {
    .x = v.x/mag,
    .y = v.y/mag,
    .z = v.z/mag
  };
  return v_normal;
}

#endif
