#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include "glad/glad.h"
#include "stb_image.h"

#define CG_MANUAL_MIPMAP_LEVEL 0

struct Texture {
  int width;
  int height;
  int nr_channels;
  unsigned int id;
};

void cg_texture_init() {
  stbi_set_flip_vertically_on_load(1);
}

void cg_load_2d_texture(struct Texture *texture, const char *texture_path, const int texture_has_alpha) {
  unsigned char *data = stbi_load(texture_path, &(texture->width), &(texture->height), &(texture->nr_channels), 0);
  if (!data) {
    printf("Failed to load texture from: %s\n", texture_path);
    stbi_image_free(data);
    return;
  }

  glGenTextures(1, &(texture->id));
  glBindTexture(GL_TEXTURE_2D, texture->id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  if (texture_has_alpha)
    glTexImage2D(GL_TEXTURE_2D, CG_MANUAL_MIPMAP_LEVEL, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  else
    glTexImage2D(GL_TEXTURE_2D, CG_MANUAL_MIPMAP_LEVEL, GL_RGB, texture->width, texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
}

void cg_bind_texture(const struct Texture texture, const int texture_type, const int texture_location) {
  glActiveTexture(texture_location);
  glBindTexture(texture_type, texture.id);
}

#endif
