#version 460 core
out vec4 FragColor;

#define MAX_AMBIENT_LIGHTS 8
#define MAX_POINTS_LIGHTS 16
#define MAX_DIRECTIONAL_LIGHTS 16
#define MAX_SPOT_LIGHTS 8

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;  
in vec3 Normal;  
  
uniform vec3 viewPos;
uniform Material material;
uniform Light pointLights[MAX_POINTS_LIGHTS];

vec3 getPointLight(Light light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);

    // diffuse shading
    float diff = max(dot(Normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    

    // combine results
    vec3 ambient  = light.ambient  * material.diffuse;
    vec3 diffuse  = light.diffuse  * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    vec3 result = ambient + diffuse + specular;
    return result;
}


void main()
{
    vec3 accumulator = vec3(0.0);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    for (int i = 0; i < MAX_POINTS_LIGHTS; i++) {
        // if light is not shining anything, skip it.
        if (pointLights[i].diffuse == vec3(0.0f)) continue;

        accumulator += getPointLight(pointLights[i], material, norm, FragPos, viewDir);
    }

    FragColor = vec4(accumulator, 1.0);
} 