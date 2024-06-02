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



vec3 Phong(vec3 worldPos, vec3 norm, vec3 view, vec3 fragColor, vec3 tangent)
{
    

    vec3 ambientColor = vec3(0.0f);
    vec3 diffuseColor = vec3(0.0f);
    vec3 specularColor = vec3(0.0f);
    for (int i = 0; i < numLights; i++)
    {

        // Ambient
        //ambientColor += lights[i].ambient;

        // Diffuse
        vec3 lightDir = normalize(lights[i].position - worldPos);
        vec3 H = normalize(lightDir + view);

        //float diff = max(dot(norm, lightDir), 0.0);
        float TdotL  = dot(lightDir, tangent);
        float TdotV = dot(view, tangent);
        float D = sqrt(1 - pow(TdotL, 2));
        float S = sqrt(1 - pow(TdotV, 2));
        
        diffuseColor += lights[i].diffuse *  D;
        //return tangent;

        // Specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = D * S - ( TdotL * TdotV);                      //pow(max(dot(view, reflectDir), 0.0), 32);
        spec = pow(spec, 5);
        specularColor += lights[i].specular * spec;
    }
    vec3 result = (ambientColor + diffuseColor + specularColor) * fragColor;
    return result;
}

vec3 Tanget(){

    vec3 pos_dx = dFdx(FragPos);
    vec3 pos_dy = dFdy(FragPos);
    // derivations of the texture coordinate
    vec2 texC_dx = dFdx(TexCoord);
    vec2 texC_dy = dFdy(TexCoord);
    // tangent vector and binormal vector
    vec3 T = texC_dy.y * pos_dx - texC_dx.y * pos_dy;
    vec3 b = texC_dx.x * pos_dy - texC_dy.x * pos_dx;

    return normalize(T);
}

void main()
{
    vec3 T = Tanget();
    vec3 norm = normalize(Normal);

    vec3 viewVec = normalize(viewPos - FragPos);
    vec3 phongColor = Phong(FragPos, norm, viewVec, texture(duckSkin, TexCoord).rgb,   T );

    //FragColor = texture(duckNormals, TexCoord) * 0.5 + texture(duckSkin, TexCoord) * 0.5;
	FragColor = vec4(phongColor, 1);
    //FragColor = vec4(norm, 1);
}