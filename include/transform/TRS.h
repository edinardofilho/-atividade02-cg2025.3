#ifndef TRS_H
#define TRS_H

#include "vector3.h"
#include "quaternion.h"
#include "matrix.h"

#define true 1
#define false 0

//This file uses a standard process common in GameEngines to decrease cpu matrix multiplication
//by using quaternions to avoid the complex rotations, allowing each step of the basic:
// Translation * Rotation * Scaling
//to use no matrices at all. A matrix conversion is still needed because GPUs only work with
//matrices, but this step is simple enough to perform. This approach also avoid Gimbal Lock
//and matrix degeneration.

typedef struct {
  Vector3 postion;
  Quatern rotation;
  Vector3 scale;
  Matrix4 cacheMatrix;
  int isDirty;
} Transform;

Transform cgTransformCreate() {
  Transform transform = {
    .postion = cgVector3Zero(),
    .rotation = cgQuaternIdentity(),
    .scale = {1.0, 1.0, 1.0},
    .cacheMatrix = cgMatrixIdentity(),
    .isDirty = false
  };
  return transform;
}

void cgTransformTranslate(Transform * const transform, Vector3 const * const translate) {
  if (translate->x == 0.0 && translate->y == 0.0 && translate->z == 0.0)
    return;

  transform->postion = cgVector3Add(&transform->postion, translate);
  transform->isDirty = true;
}

void cgTransformRotate(Transform * const transform, Vector3 const * const axis, double const degree, int isLocal) {
  if (degree == 0.0)
    return;
  
  Quatern const rot = cgQuaternRotation(axis, degree);
  if (isLocal == false)
    transform->rotation = cgQuaternProduct(&rot, &transform->rotation);
  else
    transform->rotation = cgQuaternProduct(&transform->rotation, &rot);
  transform->rotation = cgQuaternNormalize(&transform->rotation);
  transform->isDirty = true;
}

void cgTransformScaleUniform(Transform * const transform, double const scale) {
  if (scale == 1.0)
    return;

  transform->scale.x *= scale;
  transform->scale.y *= scale;
  transform->scale.z *= scale;
  transform->isDirty = true;
}

void cgTransformScaleVector3(Transform * const transform, Vector3 const * const scale) {
  if (scale->x == 1.0 && scale->y == 1.0 && scale->z == 1.0)
    return;

  transform->scale.x *= scale->x;
  transform->scale.y *= scale->y;
  transform->scale.z *= scale->z;
  transform->isDirty = true;
}

Matrix4 cgTransformGetMatrix(Transform * const transform) {
  //Only update the matrix if it was modified before called
  if (transform->isDirty == true) {
    //This process is equivalent to the general axis rotation matrix
    double x = transform->rotation.i, y = transform->rotation.j, z = transform->rotation.k, w = transform->rotation.r;
    double x2 = x + x, y2 = y + y, z2 = z + z;
    double xx = x * x2, xy = x * y2, xz = x * z2;
    double yy = y * y2, yz = y * z2, zz = z * z2;
    double wx = w * x2, wy = w * y2, wz = w * z2;

    transform->cacheMatrix.matrix[0][0] = (1.0f - (yy + zz)) * transform->scale.x;
    transform->cacheMatrix.matrix[0][1] = (xy - wz)          * transform->scale.y;
    transform->cacheMatrix.matrix[0][2] = (xz + wy)          * transform->scale.z;
    transform->cacheMatrix.matrix[0][3] = transform->postion.x;

    transform->cacheMatrix.matrix[1][0] = (xy + wz)          * transform->scale.x;
    transform->cacheMatrix.matrix[1][1] = (1.0f - (xx + zz)) * transform->scale.y;
    transform->cacheMatrix.matrix[1][2] = (yz - wx)          * transform->scale.z;
    transform->cacheMatrix.matrix[1][3] = transform->postion.y;

    transform->cacheMatrix.matrix[2][0] = (xz - wy)          * transform->scale.x;
    transform->cacheMatrix.matrix[2][1] = (yz + wx)          * transform->scale.y;
    transform->cacheMatrix.matrix[2][2] = (1.0f - (xx + yy)) * transform->scale.z;
    transform->cacheMatrix.matrix[2][3] = transform->postion.z;

    transform->cacheMatrix.matrix[3][0] = 0.0f;
    transform->cacheMatrix.matrix[3][1] = 0.0f;
    transform->cacheMatrix.matrix[3][2] = 0.0f;
    transform->cacheMatrix.matrix[3][3] = 1.0f;

    transform->isDirty = false;
  }
  return transform->cacheMatrix;
}

#endif
