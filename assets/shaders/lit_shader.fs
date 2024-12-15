#version 330 core
out vec4 FragColor;

struct DirLight {
    vec3 direction;
    vec3 color;
};

struct PointLight {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 color;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float innerCutoff;
    float outerCutoff;

    float constant;
    float linear;
    float quadratic;

    vec3 color;
};

#define MAX_DIR_LIGHTS 4
#define MAX_POINT_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform float shininess;
uniform vec3 ambient;

uniform vec3 viewPos;
uniform int numDirLights;
uniform int numPointLights;
uniform int numSpotLights;
uniform DirLight dirLights[MAX_DIR_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(FragPos - viewPos);
    vec3 result = ambient * vec3(texture(texture_diffuse1, TexCoords));

    // directional lighting
    for (int i = 0; i < numDirLights; i++) 
        result += CalcDirLight(dirLights[i], norm, viewDir);
    // point lights
    for (int i = 0; i < numPointLights; i++) 
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // spot light
    for (int i = 0; i < numSpotLights; i++) 
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * diff * vec3(texture(texture_diffuse1, TexCoords));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(/*-*/viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.color * spec * vec3(texture(texture_diffuse1, TexCoords));

    return diffuse + specular;
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // vec3 lightDir = normalize(light.position - fragPos);
    // // diffuse shading
    // float diff = max(dot(normal, lightDir), 0.0);
    // // specular shading
    // vec3 reflectDir = reflect(-lightDir, normal);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // // attenuation
    // float distance = length(light.position - fragPos);
    // float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // // combine results
    // vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    // vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    // vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    // ambient *= attenuation;
    // diffuse *= attenuation;
    // specular *= attenuation;
    // return (ambient + diffuse + specular);
    return vec3(0.0);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // vec3 lightDir = normalize(light.position - fragPos);
    // // diffuse shading
    // float diff = max(dot(normal, lightDir), 0.0);
    // // specular shading
    // vec3 reflectDir = reflect(-lightDir, normal);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // // attenuation
    // float distance = length(light.position - fragPos);
    // float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // // spotlight intensity
    // float theta = dot(lightDir, normalize(-light.direction));
    // float epsilon = light.cutOff - light.outerCutOff;
    // float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // // combine results
    // vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    // vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    // vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    // ambient *= attenuation * intensity;
    // diffuse *= attenuation * intensity;
    // specular *= attenuation * intensity;
    // return (ambient + diffuse + specular);
    return vec3(0.0);
}