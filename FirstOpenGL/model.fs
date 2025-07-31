#version 330 core
out vec4 FragColor;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float opacity;
};

uniform Material material;

in vec2 TexCoords; // interpolated
 
void main()
{
	//vec3 diffuse = vec3(texture(material.texture_diffuse1, TexCoords));
	FragColor = vec4(material.diffuse, material.opacity);
}