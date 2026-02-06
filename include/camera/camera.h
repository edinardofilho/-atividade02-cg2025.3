#ifndef CAMERA_H
#define CAMERA_H

#include "../transform/matrix.h"
#include "../transform/vector3.h"

struct Camera {
  struct Vector3 position;
  struct Vector3 forward;
  struct Vector3 right;
  struct Vector3 up;
};

struct Camera cgCameraCreate(struct Vector3 const * const position, struct Vector3 const * const target, struct Vector3 const * const worldUp) {
  struct Vector3 const targetNegative = cgVector3Negative(target);

  struct Vector3 const forward = cgVector3Add(position, &targetNegative);
  struct Vector3 const forwardNormal = cgVector3Normalize(&forward);
  
  struct Vector3 const right = cgVector3CrossProduct(worldUp, &forwardNormal);
  struct Vector3 const rightNormal = cgVector3Normalize(&right);

  struct Camera cam = {
    .position = *position,
    .forward = forwardNormal,
    .right = rightNormal,
    .up = cgVector3CrossProduct(&forwardNormal, &rightNormal)
  };
  return cam;
};

void cgCameraUpdate(struct Camera * const camera, struct Vector3 const * const position, struct Vector3 const * const target, struct Vector3 const * const worldUp) {
  struct Vector3 const targetNegative = cgVector3Negative(target);

  struct Vector3 const forward = cgVector3Add(position, &targetNegative);
  struct Vector3 const forwardNormal = cgVector3Normalize(&forward);
  
  struct Vector3 const right = cgVector3CrossProduct(worldUp, &forwardNormal);
  struct Vector3 const rightNormal = cgVector3Normalize(&right);

  camera->position = *position;
  camera->forward = forwardNormal;
  camera->right = rightNormal;
  camera->up = cgVector3CrossProduct(&forwardNormal, &rightNormal);
}

struct Matrix4 cgCameraLookAtMatrix(struct Camera const * const camera) {
  //Rotation matrices are orthogonal (perpendicular 2 by 2 and unit length) so the inverse is the transpose
  const struct Matrix4 cameraInverseRotation = {
    .matrix = {
      {camera->right.x  , camera->right.y  , camera->right.z  , 0},
      {camera->up.x     , camera->up.y     , camera->up.z     , 0},
      {camera->forward.x, camera->forward.y, camera->forward.z, 0},
      {0                , 0                , 0                , 1}
    }
  };
  const struct Matrix4 cameraInverseTranslation = {
    .matrix = {
      {1, 0, 0, -camera->position.x},
      {0, 1, 0, -camera->position.y},
      {0, 0, 1, -camera->position.z},
      {0, 0, 0, 1                  }
    }
  };
  return cgMatrixMatrixMultiplication(&cameraInverseRotation, &cameraInverseTranslation);
}

#endif
