#version 460 core 

in vec3 outVertexPos;
in vec3 outVertexNormals;
in vec2 outVertexTextureUV;

out vec4 FragColor;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 color;
  float radius;
};

uniform Light light;
uniform Material material;

uniform sampler2D texture0Data;
uniform vec3 viewPos;

void main() {
  vec3 viewDir = normalize(viewPos - outVertexPos);
  vec3 normal = normalize(outVertexNormals);

  vec3 lightDir = light.position - outVertexPos;
  float dist = length(lightDir);
  lightDir = normalize(lightDir);

  vec3 ref = reflect(-viewDir, normal);
  vec3 centerToRef = dot(lightDir, ref) * ref - lightDir;
  vec3 closest = lightDir + centerToRef * clamp(light.radius / length(centerToRef), 0.0, 1.0);
  vec3 newLightDir = normalize(closest);

  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuseLight = diff * material.diffuse;

  float spec = pow(max(dot(viewDir, newLightDir), 0.0), material.shininess);
  vec3 specularLight = spec * material.specular ;

  //float attenuation = 1.0 / (dist * dist);

  vec3 color = (material.ambient + (diffuseLight + specularLight)) * light.color;
  FragColor = texture(texture0Data, outVertexTextureUV) * vec4(color, 1.0);
  //FragColor = vec4(1.0);
}
