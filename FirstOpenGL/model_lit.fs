#version 330 core
out vec4 FragColor;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float opacity;
};

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

uniform Material material;
uniform vec3 viewPos;

// Lighting inputs
in vec3 Normal;
in vec3 FragPos;

// Reveal effect uniforms
uniform float ftime;
uniform float revealProgress;
uniform float revealPt1Speed;
uniform vec3 modelCenter;
uniform float maxModelRadius;

// Function declarations
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  

void main()
{
    // REVEAL EFFECT LOGIC
    float distFromCenter = length(FragPos - modelCenter);
    float normalizedDist = distFromCenter / maxModelRadius;
    float revealPt1Time = 1.0 / revealPt1Speed;
   
    // Discard fragments that growing threshold/veil has yet reached
    if (normalizedDist > revealProgress) {
        discard;
    }

    // LIGHTING CALCULATIONS
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // Phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    
    // Phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    // Create lit surface color
    vec4 litSurfaceColor = vec4(result, material.opacity);
    
    // REVEAL TRANSITION LOGIC
    vec4 brightWhite = vec4(1.0, 1.0, 1.0, 1.0);
    bool fullyRevealed = (revealProgress >= 1.0);
    
    if(fullyRevealed) {
        // Transition from white to lit surface
        float transitionValue = (ftime - revealPt1Time) * 0.5;
        transitionValue = clamp(transitionValue, 0.0, 1.0);
        FragColor = mix(brightWhite, litSurfaceColor, transitionValue);
    } else {
        // Show as bright white during reveal
        FragColor = brightWhite;
    }
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
    vec3 ambient  = light.ambient  * material.ambient;
    vec3 diffuse  = light.diffuse  * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
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
    vec3 ambient  = light.ambient  * material.ambient;
    vec3 diffuse  = light.diffuse  * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}