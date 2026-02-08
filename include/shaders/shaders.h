#ifndef SHADERS_H
#define SHADERS_H

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#include "../transform/matrix.h"

typedef struct {
  unsigned int id;
  float *matrixArray;
} Shaders;

Shaders cgShadersLoad(char const * const vertexPath, char const * const fragPath) {
  int const logSize = 512;
  char infoLog[logSize];
  int succ;

  //Load Shaders from file
  FILE * const vertexFile = fopen(vertexPath, "r");
  if (vertexFile == NULL) {
    printf("Unable to load vertex shader.\n");
  }

  FILE * const fragmentFile = fopen(fragPath, "r");
  if (fragmentFile == NULL) {
    printf("Unable to read fragment shader into buffer.\n");
  }
  
  //Read from file to buffer then cast to const
  char *vertexBuffer = NULL;
  size_t vertexBufferLen = 0;
  ssize_t const vertexReadReturn = getdelim(&vertexBuffer, &vertexBufferLen, '\0', vertexFile);
  if (vertexReadReturn < 0 && !feof(vertexFile))
    printf("getdelim error while reading Vertex Shader");
  vertexBuffer[vertexReadReturn - 1] = '\0';
  char const * const vertexShaderSource = vertexBuffer;

  char *fragmentBuffer = NULL;
  size_t fragmentBufferLen = 0;
  ssize_t const fragmentReadReturn = getdelim(&fragmentBuffer, &fragmentBufferLen, '\0', fragmentFile);
  if (fragmentReadReturn < 0 && !feof(fragmentFile))
    printf("getdelim error while reading Vertex Shader");
  fragmentBuffer[fragmentReadReturn - 1] = '\0';
  char const * const fragmentShaderSource = fragmentBuffer;
  
  //Compile shaders
  unsigned int const vertexId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexId, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexId);
  glGetShaderiv(vertexId, GL_COMPILE_STATUS, &succ);
  if (!succ) {
    glGetShaderInfoLog(vertexId, logSize, NULL, infoLog);
    printf("Failed to compile vertex shader:\n%s\n", infoLog);
  }
  
  unsigned int const fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentId, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentId);
  glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &succ);
  if (!succ) {
    glGetShaderInfoLog(fragmentId, logSize, NULL, infoLog);
    printf("Failed to compile fragment shader:\n%s\n", infoLog);
  }
  
  //Link shaders
  unsigned int const shadersId = glCreateProgram();
  glAttachShader(shadersId, vertexId);
  glAttachShader(shadersId, fragmentId);
  glLinkProgram(shadersId);
  glGetProgramiv(shadersId, GL_LINK_STATUS, &succ);
  if (!succ) {
    glGetProgramInfoLog(shadersId, logSize, NULL, infoLog);
    printf("Failed to link shader program:\n%s\n", infoLog);
  }
  
  //Load and pass id
  Shaders shaders = {
    .id = shadersId,
    .matrixArray = (float *)malloc(16 * sizeof(float))
  };
  if (shaders.matrixArray == NULL) {
    printf("Could not allocate matrixArray memory for shaders\n");
    abort();
  }

  //Free resources
  free(vertexBuffer);
  free(fragmentBuffer);
  glDeleteShader(vertexId);
  glDeleteShader(fragmentId);
  fclose(vertexFile);
  fclose(fragmentFile);

  return shaders;
}

void cgShadersUse(Shaders const * const shaders) {
  glUseProgram(shaders->id);
}

void cgShadersFree(Shaders const * const shaders) {
  free(shaders->matrixArray);
}

void cgShadersUniformSetInt(Shaders const * const shaders, char const * const uniformPath, int const value) {
  cgShadersUse(shaders);
  int const location = glGetUniformLocation(shaders->id, uniformPath);
  if (location == -1) {
    printf("Could not find Uniform: %s\n", uniformPath);
    return;
  }
  glUniform1i(location, value);
}

void cgShadersUniformSetFloat(Shaders const * const shaders, char const * const uniformPath, float const value) {
  cgShadersUse(shaders);
  int const location = glGetUniformLocation(shaders->id, uniformPath);
  if (location == -1) {
    printf("Could not find Uniform: %s\n", uniformPath);
    return;
  }
  glUniform1f(location, value);
}

void cgShadersUniformSetVector3(Shaders const * const shaders, char const * const uniformPath, Vector3 const * const vector) {
  cgShadersUse(shaders);
  int const location = glGetUniformLocation(shaders->id, uniformPath);
  if (location == -1) {
    printf("Could not find Uniform: %s\n", uniformPath);
    return;
  }
  float const vectorArray[] = {vector->x, vector->y, vector->z};
  glUniform3fv(location, 1, vectorArray);
}

void cgShadersUniformSetMatrix(Shaders * const shaders, char const * const uniformPath, Matrix4 const * const matrix) {
  cgShadersUse(shaders);
  cgMatrixFlatten(matrix, shaders->matrixArray);
  int const location = glGetUniformLocation(shaders->id, uniformPath);
  if (location == -1) {
    printf("Could not find Uniform: %s\n", uniformPath);
    return;
  }
  glUniformMatrix4fv(location, 1, GL_TRUE, shaders->matrixArray);
}

#endif
