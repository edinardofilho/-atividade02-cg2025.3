#ifndef TEXTURES_H
#define TEXTURES_H

#include <glad/glad.h>
#include "stb_image.h"

#define CG_MANUAL_MIPMAP_LEVEL 0

typedef struct {
  int width;
  int height;
  int nrChannels;
  unsigned int id;
} Texture;

void cgTextureInit() {
  stbi_set_flip_vertically_on_load(1);
}

Texture cgTextureLoad(char const * const texturePath, int const textureHasAlpha) {
  int width, height, nrChannels;
  unsigned char * const data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
  if (data == NULL) {
    printf("Failed to load texture from: %s\n", texturePath);
    stbi_image_free(data);
  }

  Texture texture = {
    .width = width,
    .height = height,
    .nrChannels = nrChannels,
    .id = -1
  };

  glGenTextures(1, &texture.id);
  glBindTexture(GL_TEXTURE_2D, texture.id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  if (textureHasAlpha)
    glTexImage2D(GL_TEXTURE_2D, CG_MANUAL_MIPMAP_LEVEL, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  else
    glTexImage2D(GL_TEXTURE_2D, CG_MANUAL_MIPMAP_LEVEL, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  return texture;
}

void cgTextureBind(Texture const * const texture, int const textureType, int const textureShaderLocation) {
  glActiveTexture(textureShaderLocation);
  glBindTexture(textureType, texture->id);
}

#endif
