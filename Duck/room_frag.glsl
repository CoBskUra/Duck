#version 330 core
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform int numLights;
uniform Light lights[100]; // Maximum number of lights

in vec3 FragPos;
in vec3 Normal;
in vec3 texCord;
out vec4 color;

uniform vec3 viewPos;
uniform samplerCube cubMap;


vec3 Phong(vec3 worldPos, vec3 norm, vec3 view, vec3 fragColor)
{
    vec3 ambientColor = vec3(0.0f);
    vec3 diffuseColor = vec3(0.0f);
    vec3 specularColor = vec3(0.0f);
    for (int i = 0; i < numLights; i++)
    {
        // Ambient
        ambientColor += lights[i].ambient;

        // Diffuse
        norm = normalize(norm);
        vec3 lightDir = normalize(lights[i].position - worldPos);
        float diff = max(dot(norm, lightDir), 0.0);
        diffuseColor += lights[i].diffuse * diff;

        // Specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(view, reflectDir), 0.0), 32);
        specularColor += lights[i].specular * spec;
    }
    vec3 result = (ambientColor + diffuseColor + specularColor) * fragColor;
    return result;
}


void main()
{
    vec3 result = Phong(FragPos, Normal,  normalize(viewPos - FragPos), texture(cubMap, texCord).rgb );
    color = vec4(result, 1);
}
