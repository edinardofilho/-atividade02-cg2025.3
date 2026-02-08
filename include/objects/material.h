#ifndef MATERIAL_H
#define MATERIAL_H

#include "../transform/vector3.h"

typedef struct {
  Vector3 ambient;
  Vector3 diffuse;
  Vector3 specular;
  float shininess;
} Material;

#endif
