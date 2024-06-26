#version 460 core

in vec3 FragPos;  
in vec3 Normal; 
in vec2 TexCoord;
out vec4 FragColor;

#define MAX_AMBIENT_LIGHTS 8
#define MAX_POINTS_LIGHTS 16
#define MAX_DIRECTIONAL_LIGHTS 16
#define MAX_SPOT_LIGHTS 8

struct Texture {
    sampler2D textureUnit;
    int isTextured;
    vec3 scale;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
    float transparency;
    Texture texture;
}; 

struct AmbientLight {
    vec3 color;
    float intensity;
};

struct PointLight {
    vec3 position;  
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
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

struct DirectionaLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
  
uniform vec3 viewPos;
uniform Material material;

uniform AmbientLight ambientLights[MAX_AMBIENT_LIGHTS];
uniform PointLight pointLights[MAX_POINTS_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform DirectionaLight directionalLights[MAX_DIRECTIONAL_LIGHTS];

vec3 getAmbientLight(AmbientLight light, Material material) {
    if (material.texture.isTextured == 1) {
        return light.color * light.intensity * texture(material.texture.textureUnit, TexCoord).rgb;
    } else {
        return light.color * light.intensity * material.diffuse;
    }
}

vec3 getPointLight(PointLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 TexCoord) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    vec3 ambient, diffuse, specular;

    if (material.texture.isTextured == 1) {
        ambient = light.ambient * texture(material.texture.textureUnit, TexCoord).rgb;
        diffuse = light.diffuse * diff * texture(material.texture.textureUnit, TexCoord).rgb;
        specular = light.specular * spec * texture(material.texture.textureUnit, TexCoord).rgb;
    } else {
        ambient = light.ambient * material.diffuse;
        diffuse = light.diffuse * diff * material.diffuse;
        specular = light.specular * spec * material.specular;
    }

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 getSpotLight(SpotLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 TexCoord) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // specular shading
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    vec3 ambient, diffuse, specular;

    if (material.texture.isTextured == 1) {
        ambient = light.ambient * texture(material.texture.textureUnit, TexCoord).rgb;
        diffuse = light.diffuse * diff * texture(material.texture.textureUnit, TexCoord).rgb;
        specular = light.specular * spec * texture(material.texture.textureUnit, TexCoord).rgb;
    } else {
        ambient = light.ambient * material.diffuse;
        diffuse = light.diffuse * diff * material.diffuse;
        specular = light.specular * spec * material.specular;
    }

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    
    return (ambient + diffuse + specular);
}

vec3 getDirectionaLight(DirectionaLight light, Material material, vec3 normal, vec3 viewDir, vec2 TexCoord) {
    vec3 lightDir = normalize(-light.direction);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    vec3 ambient, diffuse;

    if (material.texture.isTextured == 1) {
        ambient = light.ambient * texture(material.texture.textureUnit, TexCoord).rgb;
        diffuse = light.diffuse * diff * texture(material.texture.textureUnit, TexCoord).rgb;
    } else {
        ambient = light.ambient * material.diffuse;
        diffuse = light.diffuse * diff * material.diffuse;
    }
    
    return (ambient + diffuse);
}


void main()
{
    vec3 accumulator = vec3(0.0);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    for (int i = 0; i < MAX_AMBIENT_LIGHTS; i++) {
        // if light is not shining anything, skip it.
        if (ambientLights[i].color == vec3(0.0f)) continue;

        accumulator += getAmbientLight(ambientLights[i], material);
    }

    for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++) {
        // if light is not shining anything, skip it.
        if (directionalLights[i].diffuse == vec3(0.0f)) continue;

        accumulator += getDirectionaLight(directionalLights[i], material, norm, viewDir, TexCoord);
    }

    for (int i = 0; i < MAX_SPOT_LIGHTS; i++) {
        // if light is not shining anything, skip it.
        if (spotLights[i].diffuse == vec3(0.0f)) continue;

        accumulator += getSpotLight(spotLights[i], material, norm, FragPos, viewDir, TexCoord);
    }

    for (int i = 0; i < MAX_POINTS_LIGHTS; i++) {
        // if light is not shining anything, skip it.
        if (pointLights[i].diffuse == vec3(0.0f)) continue;

        accumulator += getPointLight(pointLights[i], material, norm, FragPos, viewDir, TexCoord);
    }

    // Gamma Correction
    accumulator = pow(accumulator, vec3(1.0/2.2));
    
    FragColor = vec4(accumulator, material.transparency);
} 