#version 460 core 

in vec3 outVertexPos;
in vec3 outVertexNormals;
in vec2 outVertexTextureUV;

out vec4 FragColor;

uniform sampler2D texture0Data;
uniform vec3 objectColor;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main() {
  //FragColor = texture(texture0Data, outVertexTextureUV) * vec4(1.0, 1.0, 1.0, 1.0);
  vec3 normal = normalize(outVertexNormals);
  vec3 lightDir = normalize(lightPos - outVertexPos);
  vec3 viewDir = normalize(viewPos - outVertexPos);

  float ambient = 0.02f;
  vec3 ambientLight = ambient * lightColor;

  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  vec3 reflectDir = reflect(-lightDir, normal);
  float specStr = 0.01;
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specStr * spec * lightColor;

  vec3 color = (ambientLight + diffuse + specular) * objectColor;
  FragColor = texture(texture0Data, outVertexTextureUV) * vec4(color, 1.0);
}
