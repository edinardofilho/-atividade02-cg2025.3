#ifndef SHADERLOADER_H
#define SHADERLOADER_H
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Shaders {
  unsigned int id;
};

void cg_load_shaders(struct Shaders *shaders, const char *vertex_path, const char *frag_path) {
  //Some boilerplate
  const int logSize = 512;
  char infoLog[logSize];
  int succ;
  
  //Load Shaders from file
  FILE *vertex_file = fopen(vertex_path, "r");
  if (vertex_file == NULL) {
    printf("Unable to load vertex shader.\n");
    return;
  }

  FILE *frag_file = fopen(frag_path, "r");
  if (frag_file == NULL) {
    printf("Unable to read fragment shader into buffer.\n");
    return;
  }
  
  //Read from file to buffer then cast to const
  char *vertex_buf = NULL;
  size_t vertex_len;
  ssize_t vertex_ret = getdelim(&vertex_buf, &vertex_len, '\0', vertex_file);
  if (vertex_ret < 0 && !feof(vertex_file)) {
    printf("getdelim error");
    abort();
  }
  vertex_buf[vertex_ret - 1] = '\0';
  const char *vertex_source = vertex_buf;

  char *frag_buf;
  size_t frag_len;
  ssize_t frag_ret = getdelim(&frag_buf, &frag_len, '\0', frag_file);
  if (frag_ret < 0 && !feof(frag_file)) {
    printf("getdelim error");
    abort();
  }
  frag_buf[frag_ret - 1] = '\0';
  const char *frag_source = frag_buf;
  
  //Compile shaders
  unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vertex_source, NULL);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &succ);
  if (!succ) {
    glGetShaderInfoLog(vertex, logSize, NULL, infoLog);
    printf("Failed to compile vertex shader:\n%s\n", infoLog);
    return;
  }
  
  unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &frag_source, NULL);
  glCompileShader(frag);
  glGetShaderiv(frag, GL_COMPILE_STATUS, &succ);
  if (!succ) {
    glGetShaderInfoLog(frag, logSize, NULL, infoLog);
    printf("Failed to compile fragment shader:\n%s\n", infoLog);
    return;
  }
  
  //Link shaders
  unsigned int id = glCreateProgram();
  glAttachShader(id, vertex);
  glAttachShader(id, frag);
  glLinkProgram(id);
  glGetProgramiv(id, GL_LINK_STATUS, &succ);
  if (!succ) {
    glGetProgramInfoLog(id, logSize, NULL, infoLog);
    printf("Failed to link shader program:\n%s\n", infoLog);
    return;
  }
  
  //Load struct and pass id
  shaders->id = id;

  //Free resources
  glDeleteShader(vertex);
  glDeleteShader(frag);
  fclose(vertex_file);
  fclose(frag_file);
}

void cg_use_shaders(const struct Shaders shaders) {
  glUseProgram(shaders.id);
}

void cg_uniform_set_int(const struct Shaders shaders, const char *path, const int value) {
  cg_use_shaders(shaders);
  glUniform1i(glGetUniformLocation(shaders.id, path), value);
}

#endif
