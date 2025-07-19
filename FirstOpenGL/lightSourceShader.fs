#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;

#define NR_POINT_LIGHTS 4  
uniform vec3 lightColors[NR_POINT_LIGHTS];
uniform int lightIndex;

void main()
{
	FragColor = vec4(lightColors[lightIndex], 1.0);
	//FragColor = vec4(1.0, 1.0, 1.0, 1.0); // set all 4 vec vals to 1.0 (constant white)
}