#ifndef LIGHT_H
#define LIGHT_H

#include "../transform/vector3.h"

typedef struct {
  Vector3 postion;
  //colors
  Vector3 ambient;
  Vector3 diffuse;
  Vector3 specular;
} Light;

#endif
