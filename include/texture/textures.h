#ifndef TEXTURES_H
#define TEXTURES_H

#include <glad/glad.h>
#include "stb_image.h"

#define CG_MANUAL_MIPMAP_LEVEL 0

struct Texture {
  int width;
  int height;
  int nrChannels;
  unsigned int id;
};

void cgTextureInit() {
  stbi_set_flip_vertically_on_load(1);
}

struct Texture cgTextureLoad(char const * const texturePath, int const textureHasAlpha) {
  int width, height, nrChannels;
  unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
  if (data == NULL) {
    printf("Failed to load texture from: %s\n", texturePath);
    stbi_image_free(data);
  }

  struct Texture texture = {
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

void cgTextureBind(struct Texture const * const texture, int const textureType, int const textureShaderLocation) {
  glActiveTexture(textureShaderLocation);
  glBindTexture(textureType, texture->id);
}

#endif
