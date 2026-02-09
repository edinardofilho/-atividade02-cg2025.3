#ifndef QUATERNION_H
#define QUATERNION_H

#include <math.h>
#include "vector3.h"

#define PI 3.14159265359

typedef struct {
  double r;
  double i;
  double j;
  double k;
} Quatern;

Quatern cgQuaternIdentity() {
  Quatern quat = {
    .r = 1.0,
    .i = 0.0,
    .j = 0.0,
    .k = 0.0
  };
  return quat;
}

Quatern cgQuaternConjugate(Quatern const * const q) {
  Quatern quat = {
    .r =  q->r,
    .i = -q->i,
    .j = -q->j,
    .k = -q->k
  };
  return quat;
}

Quatern cgQuaternProduct(Quatern const * const q1, Quatern const * const q2) {
  Quatern quat = {
    .r = q1->r*q2->r - q1->i*q2->i - q1->j*q2->j - q1->k*q2->k,
    .i = q1->r*q2->i + q1->i*q2->r + q1->j*q2->k - q1->k*q2->j,
    .j = q1->r*q2->j - q1->i*q2->k + q1->j*q2->r + q1->k*q2->i,
    .k = q1->r*q2->k + q1->i*q2->j - q1->j*q2->i + q1->k*q2->r
  };
  return quat;
}

Quatern cgQuaternNormalize(Quatern const * const q) {
  double const lenght = sqrt(q->r*q->r + q->i*q->i + q->j*q->j + q->k*q->k);
  Quatern quat = {
    .r = q->r/lenght,
    .i = q->i/lenght,
    .j = q->j/lenght,
    .k = q->k/lenght
  };
  return quat;
}

Quatern cgQuaternRotation(Vector3 const * const axis, double const degree) {
  Vector3 const axisNormalilzed = cgVector3Normalize(axis);
  double halfRad = (degree * PI / 180.0) / 2.0;
  double sine = sin(halfRad);
  Quatern quat = {
    .r = cos(halfRad),
    .i = axisNormalilzed.x * sine,
    .j = axisNormalilzed.y * sine,
    .k = axisNormalilzed.z * sine
  };
  return quat;
}
  
#endif
