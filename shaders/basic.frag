#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform sampler2D texture1;

uniform int isEmissive;


void main()
{
    vec3 color = texture(texture1, TexCoords * 2.0).rgb;

    vec3 normal = normalize(Normal);

    // AMBIENT
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * color;

    // DIFFUSE
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * color * lightColor;

    // SPECULAR
    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);

    // emissive check
    if (isEmissive == 1)
        BrightColor = vec4(result, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}