#ifndef VECTOR4_H
#define VECTOR4_H

#include <math.h>

struct Vector4 {
  double x;
  double y;
  double z;
  double w;
};

struct Vector4 cgVector4Zero() {
  struct Vector4 v_zero = {0, 0, 0, 0};
  return v_zero;
}

double cgVector4DotProduct(const struct Vector4 v1, const struct Vector4 v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

struct Vector4 cdVector4Normalize(const struct Vector4 v) {
  const double mag = sqrt(cgVector4DotProduct(v, v));
  struct Vector4 v_normal = {
    .x = v.x/mag,
    .y = v.y/mag,
    .z = v.z/mag
  };
  return v_normal;
}

#endif
