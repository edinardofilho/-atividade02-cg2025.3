#version 460 core 

in vec3 vertexOutColor;
in vec2 vertexOutTex;

out vec4 FragColor;

uniform sampler2D texture1_data;
uniform sampler2D texture2_data;

void main() {
  //FragColor = vec4(vertexOutColor, 1.0);
  FragColor = mix(texture(texture1_data, vertexOutTex), texture(texture2_data, vertexOutTex), 0.2) * vec4(vertexOutColor, 1.0);
}
