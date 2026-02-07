#ifndef SHAPES_H
#define SHAPES_H

#include "object.h"

struct Object cgShapeCreateParallelepiped(float const width, float const height, float const lenght) {
  float cubeVertices[192] = {
    //Quad1 (Z = 1)
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    //Quad2 (Z = -1)
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    //Quad3 (Y = 1)
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,

    //Quad4 (Y = -1)
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    
    //Quad5 (X = 1)
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    
    //Quad6 (X = -1)
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f
  };

  unsigned int cubeIndices[36] = {
    0, 1, 2,
    1, 2, 3,
    
    4, 5, 6,
    5, 6, 7,

    8, 9, 10,
    9, 10, 11,

    12, 13, 14,
    13, 14, 15,

    16, 17, 18,
    17, 18, 19,

    20, 21, 22,
    21, 22, 23
  };

  struct Object cube = cgObjectCreate(192, cubeVertices, 36, cubeIndices);
  struct Vector3 scaling = {width, height, lenght};
  struct Matrix4 scalingMatrix = cgMatrixVector3Scale(&scaling);
  cube.model = cgMatrixMatrixMultiplication(&scalingMatrix, &(cube.model));
  return cube;
}

struct Object cgShapeCreateSphere(float const radius, int const sectorCount, int const stackCount) {
  int vertexArraySize = 0;
  float * vertexArray = (float *)malloc(8 * (sectorCount + 1) * (stackCount + 1) * sizeof(float));
  if (vertexArray == NULL)
    printf("Could not allocate memory for sphere vertex data\n");

  float sectorStep = 2 * PI / sectorCount;
  float stackStep = PI / stackCount;

  for (int i = 0; i <= stackCount; i++) {
    float stackAngle = - PI / 2 + i * stackStep;
    float xy = cosf(stackAngle);
    float z = sinf(stackAngle);

    for (int j = 0; j <= sectorCount; j++) {
      float sectorAngle = - j * sectorStep;

      //There is a Y-Z switch because in math Z is up, but, in OpenGL, Y is up
      float x = xy * cosf(sectorAngle);
      float y = xy * sinf(sectorAngle);
      vertexArray[vertexArraySize++] = x;
      vertexArray[vertexArraySize++] = z;
      vertexArray[vertexArraySize++] = y;

      //The normals for a unit sphere are just the postion of the vertex
      //math: 
      //  The direction of a normal vector on the sphere is given by the gradient of F(x, y, z) = x² + y² + z² - r² = 0
      //  which is N = 2(x, y, z), so the direction of the normal for each vertex is the same as it's postion
      //  and since this is is a unity sphere the position vector is also normalized, so the normal and potision vectors
      //  are the same
      float nx = x;
      float ny = y;
      float nz = z;
      vertexArray[vertexArraySize++] = nx;
      vertexArray[vertexArraySize++] = nz;
      vertexArray[vertexArraySize++] = ny;

      float u = (float)j / sectorCount;
      float v = (float)i / stackCount;
      vertexArray[vertexArraySize++] = u;
      vertexArray[vertexArraySize++] = v;
    }
  }

  int indexArraySize = 0;
  unsigned int * indexArray = (unsigned int *)malloc(6 * sectorCount * stackCount * sizeof(unsigned int));
  if (vertexArray == NULL)
    printf("Could not allocate memory for sphere vertex data\n");

  for (int i = 0; i < stackCount; ++i) {
    int k1 = i * (sectorCount + 1);
    int k2 = k1 + sectorCount + 1;

    for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
      // 2 triangles per sector excluding first and last stacks
      // k1 => k2 => k1+1
      if (i != 0) {
        indexArray[indexArraySize++] = k1;
        indexArray[indexArraySize++] = k2;
        indexArray[indexArraySize++] = k1 + 1;
      }

      // k1+1 => k2 => k2+1
      if(i != (stackCount-1))
      {
        indexArray[indexArraySize++] = k1 + 1;
        indexArray[indexArraySize++] = k2;
        indexArray[indexArraySize++] = k2 + 1;
      }
    }
  }

  struct Object sphere = cgObjectCreate(vertexArraySize, vertexArray, indexArraySize, indexArray);
  struct Vector3 scaling = {radius, radius, radius};
  struct Matrix4 scalingMatrix = cgMatrixVector3Scale(&scaling);
  sphere.model = cgMatrixMatrixMultiplication(&scalingMatrix, &(sphere.model));
  return sphere;
}

#endif
