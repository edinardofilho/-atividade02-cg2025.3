#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>

#include "../transform/matrix.h"

struct Object {
  unsigned int vao;
  unsigned int vbo;
  unsigned int ebo;
  unsigned int elementCount;
  unsigned int vertexCount;
  unsigned int *elementBuffer;
  float *vertexBuffer;
  struct Matrix4 model;
};

int cgObjectCreate(struct Object *objectPtr, unsigned int const vertexCount, float * const vertexData, unsigned int const indexCount, unsigned int * const indexData) {
  objectPtr->vertexCount = vertexCount;
  objectPtr->vertexBuffer = vertexData;
  objectPtr->elementCount = indexCount;
  objectPtr->elementBuffer = indexData;
  objectPtr->model = cgMatrixIdentity();

  glGenVertexArrays(1, &(objectPtr->vao));
  glGenBuffers(1, &(objectPtr->vbo));
  glGenBuffers(1, &(objectPtr->ebo));
  
  return 1;
}

void cgObjectSendRenderData(struct Object const * const object) {
  glBindVertexArray(object->vao);
  
  glBindBuffer(GL_ARRAY_BUFFER, object->vbo);
  glBufferData(GL_ARRAY_BUFFER, 8 * object->vertexCount * sizeof(float), object->vertexBuffer, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, object->elementCount * sizeof(unsigned int), object->elementBuffer, GL_STATIC_DRAW);
  
  //Vertice data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  //Normal data
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  //UV data
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
}

void cgObjectDraw(const struct Object *object) {
  glBindVertexArray(object->vao);
  glDrawElements(GL_TRIANGLES, object->elementCount, GL_UNSIGNED_INT, 0);
}

#endif
