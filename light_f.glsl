#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform float cutOff;
uniform float outerCutOff;
uniform vec3 spotDir;
uniform vec3 lightPos;
uniform vec3 viewPos; 
uniform vec3 lightColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float theta = dot(lightDir, normalize(-spotDir)); 
    float epsilon = cutOff - outerCutOff;
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);
    vec3 directional = diffuse * intensity;

    FragColor = vec4(ambient + diffuse * 0.5 + directional, 1.0) * texture(texture_diffuse1, TexCoords);
} 

