#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0)); // get vertex in world space coords for lighting calc
	// normal matrix defiend as transpose of the inverse of the upper-left 3x3 part of model matrix.
	// only needed for non-uniform scaling that may occur
	// inversing matrices costly for shaders. better way is to calculate on cpu and send here as uniform
	Normal = mat3(transpose(inverse(model))) * aNormal;
	
	//Normal = aNormal; // fine if no non-uniform scaling
	TexCoords = aTexCoords;
}