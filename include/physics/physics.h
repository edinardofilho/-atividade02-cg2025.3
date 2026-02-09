#ifndef PHYSICS_H
#define PHYSICS_H

#include "../transform/vector3.h"
#include "../objects/object.h"
#include "../objects/shapes.h"
#include "../objects/material.h"
#include "../objects/light.h"

#define G_CONST 6.6743e-5

typedef union {
  Material material;
  Light light;
} PhysicsType;

typedef struct {
  double mass;
  Vector3 speed;
  Vector3 acceleration;
  PhysicsType type;
  Object obj;
} PhysicsObject;

PhysicsObject cgPhysicsCreateMaterial(double const radius, double const mass, Vector3 const * const initialPostion, Vector3 const * const initialSpeed, Material const * const material) {
  PhysicsObject pobj = {
    .mass = mass,
    .speed = *initialSpeed,
    .acceleration = cgVector3Zero(),
    .type.material = *material,
    .obj = cgShapeCreateSphere(radius)
  };
  cgTransformTranslate(&(pobj.obj.transform), initialPostion);
  return pobj;
}

PhysicsObject cgPhysicsCreateLight(double const radius, double const mass, Vector3 const * const initialPostion, Vector3 const * const initialSpeed, Light const * const light) {
  PhysicsObject pobj = {
    .mass = mass,
    .speed = *initialSpeed,
    .acceleration = cgVector3Zero(),
    .type.light = *light,
    .obj = cgShapeCreateSphere(radius)
  };
  cgTransformTranslate(&(pobj.obj.transform), initialPostion);
  return pobj;
}

void cgPhysicsUpdateForces(PhysicsObject * const physicsObjArray, unsigned int const arraySize) {
  for (int i = 0; i < arraySize; i++)
    physicsObjArray[i].acceleration = cgVector3Zero();

  for (int i = 0; i < arraySize; i++) {
    for (int j = i + 1; j < arraySize; j++) {
      Vector3 posIneg = cgVector3Negative(&physicsObjArray[i].obj.transform.postion);
      Vector3 distance = cgVector3Add(&(physicsObjArray[j].obj.transform.postion), &posIneg);
      Vector3 distanceDir = cgVector3Normalize(&distance);

      double gravity = G_CONST * physicsObjArray[i].mass * physicsObjArray[j].mass / cgVector3DotProduct(&distance, &distance);

      Vector3 accel = cgVector3ScalarProduct(gravity / physicsObjArray[i].mass, &distanceDir);
      physicsObjArray[i].acceleration = cgVector3Add(&accel, &(physicsObjArray[i].acceleration));

      Vector3 accelBack = cgVector3ScalarProduct(-gravity / physicsObjArray[j].mass, &distanceDir);
      physicsObjArray[j].acceleration = cgVector3Add(&accelBack, &(physicsObjArray[j].acceleration));
    }
  }
}

void cgPhysicsUpdatePosition(PhysicsObject * const physicsObjArray, unsigned int const arraySize, double const timeSlice) {
  for (int i = 0; i < arraySize; i++) {
    Vector3 const speedSlice = cgVector3ScalarProduct(timeSlice, &(physicsObjArray[i].acceleration));
    physicsObjArray[i].speed = cgVector3Add(&speedSlice, &(physicsObjArray[i].speed));
    Vector3 const positionSlice = cgVector3ScalarProduct(timeSlice, &(physicsObjArray[i].speed));
    cgTransformTranslate(&(physicsObjArray[i].obj.transform), &positionSlice);
  }
}

#endif
