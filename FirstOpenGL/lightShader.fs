#version 330 core
out vec4 FragColor;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light{
	vec3 position;
	vec3 direction;
	float cutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform Light light;

uniform vec3 objectColor;
uniform vec3 viewPos;

in vec3 Normal;
// per fragment world pos
in vec3 FragPos; // GPU takes 3 triangle vertices and interpolates between them to generate a unique FragPos value for every fragment/pixel that lies within the triangle.
in vec2 TexCoords;

void main()
{
	



	vec3 lightDir = normalize(light.position - FragPos); // from frag to light source

	// spotlight calcs
	// as angle increases, theta gets smaller bc its cos val. thus angle must be smaller, for greater theta
	float theta = dot(lightDir, normalize(-light.direction)); // negate to point towards light source
	if(theta > light.cutOff) // greater than bc using cos values, where angle of 0 dgrees = cos val 1
	{

	// usually dont care abt magnitude of a vec or pos, only direction for lighting
	// so normalize to simplify calculations
	vec3 norm = normalize(Normal);
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	//vec3 lightDir = normalize(-light.direction); // pointing towards light source

	// specular calcs
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // last param is shininess 
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	// max bc dot prod will go neg if angle greater than 90
	// if orthogonal, then means light ray is parllel to surface thus 0 diff
	float diff = max(dot(norm, lightDir), 0.0); // diffuse impact of light on current frag
	// greater the angle, darker the diffuse 
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); // sample material.diffuse texture at texcoords ()

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)); // ambient controlled with light

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	// (HACK) sample the emission texture only where the specular map is black (the middle of the crate)
	//vec3 specularTexel = texture(material.specular, TexCoords).rgb;
	//vec3 emission = specularTexel == vec3(0.0) ? texture(material.emission, TexCoords).rgb : vec3(0.0);
	vec3 result = (ambient + diffuse + specular);
	FragColor = vec4(result, 1.0);
	}

	else
		FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
}