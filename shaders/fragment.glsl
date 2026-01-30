#version 460 core 

in vec3 vertexOutColor;
out vec4 FragColor;

void main() {
  FragColor = vec4(vertexOutColor, 1.0);
}
