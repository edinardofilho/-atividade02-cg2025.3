#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTex;

out vec3 vertexOutColor;
out vec2 vertexOutTex;

void main() {
  gl_Position = vec4(inPos, 1.0);
  vertexOutColor = inColor;
  vertexOutTex = inTex;
}
