#version 460 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormals;
layout (location = 2) in vec2 inTextureUV;

out vec3 outVertexPos;
out vec3 outVertexNormals;
out vec2 outVertexTextureUV;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main() {
  outVertexTextureUV = inTextureUV;

  outVertexNormals = mat3(transpose(inverse(model))) * inNormals;

  outVertexPos = vec3(model * vec4(inPosition, 1.0));
  gl_Position = projection * view * vec4(outVertexPos, 1.0);
}
