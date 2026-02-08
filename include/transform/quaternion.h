#ifndef QUATERNION_H
#define QUATERNION_H

#include "vector3.h"
#include "matrix.h"

typedef struct {
  double r;
  double i;
  double j;
  double k;
} Quatern;

Quatern cgQuaternConjugate(const Quatern q) {
  Quatern quat = {
    .r = q.r,
    .i = -q.i,
    .j = -q.j,
    .k = -q.k
  };
  return quat;
}

Quatern cgQuaternProduct(const Quatern q1, const Quatern q2) {
  Quatern quat = {
    .r = q1.r*q2.r - q1.i*q2.i - q1.j*q2.j - q1.k*q2.k,
    .i = q1.r*q2.i + q1.i*q2.r + q1.j*q2.k - q1.k*q2.j,
    .j = q1.r*q2.j - q1.i*q2.k + q1.j*q2.r + q1.k*q2.i,
    .k = q1.r*q2.k + q1.i*q2.j - q1.j*q2.i + q1.k*q2.r
  };
  return quat;
}

Quatern cgQuaternUnitQuaternionVector3(const Vector3 vector) {
  const Vector3 vectorNormalilzed = cgVector3Normalize(&vector);
  Quatern quat = {
    .r = 0.0,
    .i = vectorNormalilzed.x,
    .j = vectorNormalilzed.y,
    .k = vectorNormalilzed.z
  };
  return quat;
}
  
#endif
