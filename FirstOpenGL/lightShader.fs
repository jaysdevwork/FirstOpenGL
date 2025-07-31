#version 330 core
out vec4 FragColor;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

// need to change main.cpp to account for this multiple light approach
struct DirLight{
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;

struct PointLight{    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct SpotLight{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};
uniform SpotLight spotLight;


uniform Material material;

uniform vec3 viewPos;

in vec3 Normal;
// per fragment world pos
in vec3 FragPos; // GPU takes 3 triangle vertices and interpolates between them to generate a unique FragPos value for every fragment/pixel that lies within the triangle.
in vec2 TexCoords;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float near = 0.1;
float far = 100.0;

// transform non-linear depth in screen space to linear depth value
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to ndc -1 to 1
    return (2.0 * near * far) / (far + near - z * (far - near)); // inverse non-linear equation
}

void main()
{
	// properties
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// phase 1: Directional lighting
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	// phase 2: Point lights
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}
	// phase 3: Spot light
	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

	//FragColor = vec4(result, 1.0);
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // convert to range 0,1
    FragColor = vec4(vec3(depth), 1.0);
    

}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}  


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos); // from frag to light source

    // spotlight calcs
    // as angle increases, theta gets smaller bc its cos val. thus angle must be smaller, for greater theta
    float theta = dot(lightDir, normalize(-light.direction)); // negate to point towards light source
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    //vec3 lightDir = normalize(-light.direction); // pointing towards light source

    // specular calcs
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // last param is shininess 
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // max bc dot prod will go neg if angle greater than 90
    // if orthogonal, then means light ray is parllel to surface thus 0 diff
    float diff = max(dot(normal, lightDir), 0.0); // diffuse impact of light on current frag
    // greater the angle, darker the diffuse 
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); // sample material.diffuse texture at texcoords ()

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)); // ambient controlled with light

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // for spotlight smooth edges
    diffuse *= intensity;
    specular *= intensity;

    // (HACK) sample the emission texture only where the specular map is black (the middle of the crate)
    //vec3 specularTexel = texture(material.specular, TexCoords).rgb;
    //vec3 emission = specularTexel == vec3(0.0) ? texture(material.emission, TexCoords).rgb : vec3(0.0);
    vec3 result = (ambient + diffuse + specular);
    return result;
}