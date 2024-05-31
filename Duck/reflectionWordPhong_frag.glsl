#version 330 core

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform int numLights;
uniform Light lights[100]; // Maximum number of lights
uniform mat4 reflectPlaneModelMtx_invers;

in vec3 FragPos;
in vec3 Normal;
in vec3 SurfaceColor;
out vec4 color;

uniform vec3 viewPos;
uniform float mirrorSide; // mówi czy przed lustrem czy za

bool ShouldDrawOnMirror(){
    vec3 viewer_RelatedToMirror = vec3(reflectPlaneModelMtx_invers * vec4(viewPos, 1));
    vec3 FragPos_RelatedToMirror = vec3(reflectPlaneModelMtx_invers * vec4(FragPos, 1));

    return mirrorSide * viewer_RelatedToMirror.z * FragPos_RelatedToMirror.z < 0;
 }

void main()
{
    if(!ShouldDrawOnMirror())
        discard;

    vec3 ambientColor = vec3(0.0f);
    vec3 diffuseColor = vec3(0.0f);
    vec3 specularColor = vec3(0.0f);
    for (int i = 0; i < numLights; i++)
    {
        // Ambient
        ambientColor += lights[i].ambient * SurfaceColor;
        // Diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lights[i].position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        diffuseColor += lights[i].diffuse * diff * SurfaceColor;
        // Specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        specularColor += lights[i].specular * spec;
    }
    vec3 result = ambientColor + diffuseColor + specularColor;
    color = vec4(result, 1.0f);
}
