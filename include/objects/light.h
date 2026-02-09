#ifndef LIGHT_H
#define LIGHT_H

#include "../transform/vector3.h"

typedef struct {
  Vector3 position;
  Vector3 color;
  float radius;
} Light;

#endif
