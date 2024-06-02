#version 330 core

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform int numLights;
uniform Light lights[100]; // Maximum number of lights



// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;


uniform vec3 viewPos;
uniform sampler2D duckSkin;
uniform sampler2D duckNormals;

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

vec3 Tanget(){
    vec3 dPdx = dFdx(FragPos);
    vec3 dPdy = dFdy(FragPos);
    vec2 dtdx = dFdx(TexCoord);
    vec2 dtdy = dFdy(TexCoord);
    vec3 T = normalize(-dPdx*dtdy.y + dPdy*dtdx.y);

    return T;
}

void main()
{
    vec3 T = Tanget();
    vec3 tn = texture(duckNormals, TexCoord).rgb;
    vec3 newNormal = normalMapping(Normal, T, tn * 2.0f - 1.0);
    vec3 viewVec = normalize(viewPos - FragPos);
    vec3 phongColor = Phong(FragPos, newNormal, viewVec, texture(duckSkin, TexCoord).rgb );

    //FragColor = texture(duckNormals, TexCoord) * 0.5 + texture(duckSkin, TexCoord) * 0.5;
	FragColor = vec4(phongColor, 1);
}