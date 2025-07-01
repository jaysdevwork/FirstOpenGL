#version 330 core
out vec4 FragColor;

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

uniform vec3 objectColor;
uniform vec3 viewPos;

in vec3 Normal;
// per fragment world pos
in vec3 FragPos; // GPU takes 3 triangle vertices and interpolates between them to generate a unique FragPos value for every fragment/pixel that lies within the triangle.


void main()
{
	// usually dont care abt magnitude of a vec or pos, only direction for lighting
	// so normalize to simplify calculations
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos); // from frag to light source

	// specular calcs
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // last param is shininess 
	vec3 specular = light.specular * (spec * material.specular);

	// max bc dot prod will go neg if angle greater than 90
	// if orthogonal, then means light ray is parllel to surface thus 0 diff
	float diff = max(dot(norm, lightDir), 0.0); // diffuse impact of light on current frag
	// greater the angle, darker the diffuse 
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	vec3 ambient = light.ambient * material.ambient;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}