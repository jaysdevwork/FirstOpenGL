#version 330 core
out vec4 FragColor;

struct Material{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	float shininess;
};

uniform Material material;

in vec2 TexCoords; // interpolated
 
void main()
{
	vec3 diffuse = vec3(texture(material.texture_diffuse1, TexCoords));
	FragColor = vec4(diffuse, 1.0);
}