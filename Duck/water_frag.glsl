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
in vec3 SurfaceColor;
in vec2 texCord;


uniform sampler2D heightMap;

out vec4 color;

uniform vec3 viewPos;
uniform float codeWaterHeight;
uniform float maxWaterHeight;

vec3 normalMapping(vec3 N, vec3 T, vec3 tn)
{
    N = normalize(N);
    vec3 B = cross(N, T);
    B = normalize(B);
    T = cross(N, N);
    T = normalize(T);
    tn = normalize(tn);
	
    mat3 TBN = mat3(T, B, N);
    vec3 world_normal = TBN * tn;
 
    return normalize(world_normal);
}

vec3 Phong(vec3 worldPos, vec3 norm, vec3 view)
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
    vec3 result = (ambientColor + diffuseColor + specularColor) * SurfaceColor;
    return result;
}

void main()
{
    vec3 pos = FragPos;
    pos.y += texture(heightMap, texCord).a * codeWaterHeight * maxWaterHeight;
    vec3 view = normalize(viewPos - pos);
    vec3 N = normalize(Normal);
    vec3 dPdx = dFdx(pos);
    vec3 dPdy = dFdy(pos);
    vec2 dtdx = dFdx(texCord);
    vec2 dtdy = dFdy(texCord);
    vec3 T = normalize(-dPdx*dtdy.y + dPdy*dtdx.y);

    vec3 tn = normalize(texture(heightMap, texCord).rgb);
    N = normalMapping(N, T, tn * 2.0f - 1.0);
    vec3 result = Phong(pos, N, view);
    color = vec4(result, 1) ;
    //color = vec4(texture(heightMap, texCord).a, 1, 1,1) ;
}

