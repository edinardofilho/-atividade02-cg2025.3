#version 460 core 

in vec2 outVertexTextureUV;

out vec4 FragColor;

uniform sampler2D texture0Data;

void main() {
  //FragColor = vec4(1.0, 1.0, 1.0, 1.0);
  FragColor = texture(texture0Data, outVertexTextureUV) * vec4(1.0, 1.0, 1.0, 1.0);
}
