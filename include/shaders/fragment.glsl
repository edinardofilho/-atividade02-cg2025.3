#version 460 core 

in vec2 outVertexTextureUV;

out vec4 FragColor;

uniform sampler2D texture0Data;
uniform sampler2D texture1Data;

void main() {
  //FragColor = vec4(1.0, 1.0, 1.0, 1.0);
  FragColor = mix(texture(texture0Data, outVertexTextureUV), texture(texture1Data, outVertexTextureUV), 0.2) * vec4(1.0, 1.0, 1.0, 1.0);
}
