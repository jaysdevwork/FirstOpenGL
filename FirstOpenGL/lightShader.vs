#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

uniform vec3 objectColor;
uniform vec3 lightColor;

//out vec3 Normal;
//out vec3 FragPos;
//out vec3 LightPos;
out vec3 Result;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vec3 FragPos = vec3(view * model * vec4(aPos, 1.0)); // get vertex in world space coords for lighting calc
	// normal matrix defiend as transpose of the inverse of the upper-left 3x3 part of model matrix.
	// only needed for non-uniform scaling that may occur
	// inversing matrices costly for shaders. better way is to calculate on cpu and send here as uniform
	vec3 Normal = mat3(transpose(inverse(view * model))) * aNormal;
	vec3 LightPos = vec3(view * vec4(lightPos, 1.0)); // transform world-space light pos to view-space light pos
	
	//Normal = mat3(model) * aNormal; // fine if no non-uniform scaling


	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos); // from frag to light source

	// specular calcs
	float specularStrength = 0.5;
	vec3 viewDir = normalize(- FragPos); // viewer is always at vec3(0) in viewspace, so vec3 - FragPos
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // last param is shininess 
	vec3 specular = specularStrength * spec * lightColor;

	// max bc dot prod will go neg if angle greater than 90
	// if orthogonal, then means light ray is parllel to surface thus 0 diff
	float diff = max(dot(norm, lightDir), 0.0); // diffuse impact of light on current frag
	// greater the angle, darker the diffuse 
	vec3 diffuse = diff * lightColor;

	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	Result = (ambient + diffuse + specular) * objectColor;
}