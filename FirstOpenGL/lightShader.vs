#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(view * model * vec4(aPos, 1.0)); // get vertex in world space coords for lighting calc
	// normal matrix defiend as transpose of the inverse of the upper-left 3x3 part of model matrix.
	// only needed for non-uniform scaling that may occur
	// inversing matrices costly for shaders. better way is to calculate on cpu and send here as uniform
	Normal = mat3(transpose(inverse(view * model))) * aNormal;
	LightPos = vec3(view * vec4(lightPos, 1.0)); // transform world-space light pos to view-space light pos
	
	//Normal = mat3(model) * aNormal; // fine if no non-uniform scaling
}