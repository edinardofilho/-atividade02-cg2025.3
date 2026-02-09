#version 460 core 

in vec3 outVertexPos;
in vec3 outVertexNormals;
in vec2 outVertexTextureUV;

out vec4 FragColor;

uniform sampler2D texture0Data;

void main()
{             
    FragColor = texture(texture0Data, outVertexTextureUV) * vec4(1.0);
}
