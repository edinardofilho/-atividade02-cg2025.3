#ifndef CAMERA_H
#define CAMERA_H

#include "../transform/matrix.h"
#include "../transform/vector3.h"

typedef struct {
  Vector3 position;
  Vector3 forward;
  Vector3 right;
  Vector3 up;
} Camera;

Camera cgCameraCreate(Vector3 const * const position, Vector3 const * const target, Vector3 const * const worldUp) {
  Vector3 const targetNegative = cgVector3Negative(target);

  Vector3 const forward = cgVector3Add(position, &targetNegative);
  Vector3 const forwardNormal = cgVector3Normalize(&forward);
  
  Vector3 const right = cgVector3CrossProduct(worldUp, &forwardNormal);
  Vector3 const rightNormal = cgVector3Normalize(&right);

  Camera cam = {
    .position = *position,
    .forward = forwardNormal,
    .right = rightNormal,
    .up = cgVector3CrossProduct(&forwardNormal, &rightNormal)
  };
  return cam;
};

void cgCameraUpdate(Camera * const camera, Vector3 const * const position, Vector3 const * const target, Vector3 const * const worldUp) {
  Vector3 const targetNegative = cgVector3Negative(target);

  Vector3 const forward = cgVector3Add(position, &targetNegative);
  Vector3 const forwardNormal = cgVector3Normalize(&forward);
  
  Vector3 const right = cgVector3CrossProduct(worldUp, &forwardNormal);
  Vector3 const rightNormal = cgVector3Normalize(&right);

  camera->position = *position;
  camera->forward = forwardNormal;
  camera->right = rightNormal;
  camera->up = cgVector3CrossProduct(&forwardNormal, &rightNormal);
}

Matrix4 cgCameraLookAtMatrix(Camera const * const camera) {
  //Rotation matrices are orthogonal (perpendicular 2 by 2 and unit length) so the inverse is the transpose
  Matrix4 const cameraInverseRotation = {
    .matrix = {
      {camera->right.x  , camera->right.y  , camera->right.z  , 0},
      {camera->up.x     , camera->up.y     , camera->up.z     , 0},
      {camera->forward.x, camera->forward.y, camera->forward.z, 0},
      {0                , 0                , 0                , 1}
    }
  };
  Matrix4 const cameraInverseTranslation = {
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
