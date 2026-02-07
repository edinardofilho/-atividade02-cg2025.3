#ifndef SHADERS_H
#define SHADERS_H

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#include "../transform/matrix.h"

struct Shaders {
  unsigned int id;
  float *matrixArray;
};

struct Shaders cgShadersLoad(char const * const vertexPath, char const * const fragPath) {
  const int logSize = 512;
  char infoLog[logSize];
  int succ;

  //Load Shaders from file
  FILE *vertexFile = fopen(vertexPath, "r");
  if (vertexFile == NULL) {
    printf("Unable to load vertex shader.\n");
  }

  FILE *fragmentFile = fopen(fragPath, "r");
  if (fragmentFile == NULL) {
    printf("Unable to read fragment shader into buffer.\n");
  }
  
  //Read from file to buffer then cast to const
  char *vertexBuffer = NULL;
  size_t vertexBufferLen = 0;
  ssize_t vertexReadReturn = getdelim(&vertexBuffer, &vertexBufferLen, '\0', vertexFile);
  if (vertexReadReturn < 0 && !feof(vertexFile))
    printf("getdelim error while reading Vertex Shader");
  vertexBuffer[vertexReadReturn - 1] = '\0';
  const char *vertexShaderSource = vertexBuffer;

  char *fragmentBuffer = NULL;
  size_t fragmentBufferLen = 0;
  ssize_t fragmentReadReturn = getdelim(&fragmentBuffer, &fragmentBufferLen, '\0', fragmentFile);
  if (fragmentReadReturn < 0 && !feof(fragmentFile))
    printf("getdelim error while reading Vertex Shader");
  fragmentBuffer[fragmentReadReturn - 1] = '\0';
  const char *fragmentShaderSource = fragmentBuffer;
  
  //Compile shaders
  unsigned int vertexId = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexId, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexId);
  glGetShaderiv(vertexId, GL_COMPILE_STATUS, &succ);
  if (!succ) {
    glGetShaderInfoLog(vertexId, logSize, NULL, infoLog);
    printf("Failed to compile vertex shader:\n%s\n", infoLog);
  }
  
  unsigned int fragmentId = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentId, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentId);
  glGetShaderiv(fragmentId, GL_COMPILE_STATUS, &succ);
  if (!succ) {
    glGetShaderInfoLog(fragmentId, logSize, NULL, infoLog);
    printf("Failed to compile fragment shader:\n%s\n", infoLog);
  }
  
  //Link shaders
  unsigned int shadersId = glCreateProgram();
  glAttachShader(shadersId, vertexId);
  glAttachShader(shadersId, fragmentId);
  glLinkProgram(shadersId);
  glGetProgramiv(shadersId, GL_LINK_STATUS, &succ);
  if (!succ) {
    glGetProgramInfoLog(shadersId, logSize, NULL, infoLog);
    printf("Failed to link shader program:\n%s\n", infoLog);
  }
  
  //Load struct and pass id
  struct Shaders shaders = {
    .id = shadersId,
    .matrixArray = (float *)malloc(16 * sizeof(float))
  };
  if (shaders.matrixArray == NULL) {
    printf("Could not allocate matrixArray memory for shaders\n");
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

void cgShadersUse(struct Shaders const * const shaders) {
  glUseProgram(shaders->id);
}

void cgShadersFree(struct Shaders const * const shaders) {
  free(shaders->matrixArray);
}

void cgShadersUniformSetInt(struct Shaders const * const shaders, char const * const uniformPath, int const value) {
  cgShadersUse(shaders);
  int location = glGetUniformLocation(shaders->id, uniformPath);
  if (location == -1) {
    printf("Could not find Uniform: %s\n", uniformPath);
    return;
  }
  glUniform1i(location, value);
}

void cgShadersUniformSetFloat(struct Shaders const * const shaders, char const * const uniformPath, float const value) {
  cgShadersUse(shaders);
  int location = glGetUniformLocation(shaders->id, uniformPath);
  if (location == -1) {
    printf("Could not find Uniform: %s\n", uniformPath);
    return;
  }
  glUniform1f(location, value);
}

void cgShadersUniformSetMatrix(struct Shaders * const shaders, char const * const uniformPath, struct Matrix4 const * const matrix) {
  cgMatrixFlatten(matrix, shaders->matrixArray);
  cgShadersUse(shaders);
  int location = glGetUniformLocation(shaders->id, uniformPath);
  if (location == -1) {
    printf("Could not find Uniform: %s\n", uniformPath);
    return;
  }
  glUniformMatrix4fv(location, 1, GL_TRUE, shaders->matrixArray);
}

#endif
