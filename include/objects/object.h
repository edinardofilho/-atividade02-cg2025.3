#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../transform/matrix.h"
#include "../shaders/shaders.h"

typedef struct {
  unsigned int vao;
  unsigned int vbo;
  unsigned int ebo;
  unsigned int eboSize;
  unsigned int vboSize;
  unsigned int *elementBuffer;
  float *vertexBuffer;
  Matrix4 model;
} Object;

Object cgObjectCreate(unsigned int const vertexArraySize, float * const vertexData,
                      unsigned int const indexArraySize, unsigned int * const indexData) {
  float * reservedVertexData = malloc(vertexArraySize * sizeof(float));
  if (reservedVertexData == NULL) {
    printf("Could not reserve vertex data\n");
    abort();
  }
  memcpy(reservedVertexData, vertexData, vertexArraySize * sizeof(float));

  unsigned int * reservedIndexData = malloc(indexArraySize * sizeof(unsigned int));
  if (reservedIndexData == NULL) {
    printf("Could not reserve index data\n");
    abort();
  }
  memcpy(reservedIndexData, indexData, indexArraySize * sizeof(unsigned int));

  Object object = {
    .vao = -1,
    .vbo = -1,
    .ebo = -1,
    .vboSize = vertexArraySize,
    .vertexBuffer = reservedVertexData,
    .eboSize = indexArraySize,
    .elementBuffer = reservedIndexData,
    .model = cgMatrixIdentity()
  };

  glGenVertexArrays(1, &(object.vao));
  glGenBuffers(1, &(object.vbo));
  glGenBuffers(1, &(object.ebo));

  glBindVertexArray(object.vao);
  
  glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
  glBufferData(GL_ARRAY_BUFFER, object.vboSize * sizeof(float), object.vertexBuffer, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, object.eboSize * sizeof(unsigned int), object.elementBuffer, GL_STATIC_DRAW);
  
  //Vertice data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0                ));
  glEnableVertexAttribArray(0);
  //Normal data
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  //UV data
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
  return object;
}

void cgObjectDraw(Shaders const * const shaders, Object const * const object) {
  glUseProgram(shaders->id);
  glBindVertexArray(object->vao);
  glDrawElements(GL_TRIANGLES, object->eboSize, GL_UNSIGNED_INT, 0);
}

#endif
