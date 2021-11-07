#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

//漫反射纹理
uniform sampler2D texture_diffuse1;
//高光纹理

void main()
{    
	FragColor = vec4(normalize(Normal), 1.0f);
}