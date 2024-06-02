#version 330 core
struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform int numLights;
uniform Light lights[100]; // Maximum number of lights

in vec3 localPos;
in vec3 FragPos;
in vec3 Normal;
in vec2 texCord;


uniform sampler2D heightMap;
uniform samplerCube cubMap;

out vec4 color;

uniform vec3 viewPos;
uniform float codeWaterHeight;
uniform float maxWaterHeight;

// vec3 normalMapping(vec3 N, vec3 T, vec3 tn)
// {
//     N = normalize(N);
//     vec3 B = cross(N, T);
//     B = normalize(B);
//     T = cross(N, N);
//     T = normalize(T);
//     tn = normalize(tn);
// 	
//     mat3 TBN = mat3(T, B, N);
//     vec3 world_normal = TBN * tn;
//  
//     return normalize(world_normal);
// }

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



vec3 intersectRay(vec3 p, vec3 d)
{
    vec3 one = vec3(1.0f, 1.0f, 1.0f);
    
    vec3 t1 = (one - p) / d;
    vec3 t2 = (-one - p) / d;
    
    vec3 tm = max(t1, t2);
    float t = min(min(tm.x, tm.y), tm.z);
    
    return normalize (p + t * d);
}

float fresnel(vec3 N, vec3 V, float n1, float n2)
{
    float n12 = (n2 - n1) / (n2 + n1);
    float F0 = n12 * n12;
    float cosTheta = max(dot(N, V), 0.0f);
    
    return F0 + (1 - F0) * pow(1.0f - cosTheta, 5.0f);
}

void main()
{
// frag position update
    vec3 pos = FragPos;
    pos.y += texture(heightMap, texCord).a * codeWaterHeight * maxWaterHeight ; // by³o fajne jak u¿ywaliœmy phonga
//  new normal vector
    vec3 viewVec = normalize(viewPos - pos);
                                                                    // niepotrzebne bo nie ma zakrzywieñ powierzchni
                                                                    //vec3 N = normalize(Normal);
                                                                    //    vec3 dPdx = dFdx(pos);
                                                                    //    vec3 dPdy = dFdy(pos);
                                                                    //    vec2 dtdx = dFdx(texCord);
                                                                    //    vec2 dtdy = dFdy(texCord);
                                                                    //    vec3 T = normalize(-dPdx*dtdy.y + dPdy*dtdx.y);
                                                                    //
    vec3 tn = texture(heightMap, texCord).rgb;
                                                                    //    N = normalMapping(N, T, tn * 2.0f - 1.0);
                                                                            

// cubMap mapping
    vec3 norm = normalize(tn * 2.0f - 1.0);
    
    float n1 = 1.0f;
    float n2 = 4.0f / 3.0f;
    
   
    float n = dot(norm, viewVec) > 0 ? n1 / n2 : n2 / n1;
    if (dot(norm, viewVec) < 0)
        norm *= -1.0f;
    
    vec3 reflectVec = reflect(-viewVec, norm);
    vec3 refractVec = refract(-viewVec, norm, n);
    
    vec3 texReflectCord = intersectRay(localPos, reflectVec);
    vec3 reflectColor = vec3(texture(cubMap, texReflectCord));
    
    vec3 texRefractCord = intersectRay(localPos, refractVec);
    vec3 refractColor = vec3 (texture(cubMap, texRefractCord));
    
    float f = fresnel(norm, viewVec, n1, n2);
    
    vec3 result = (refractVec.x <= 0.0f || refractVec.y <= 0.0f || refractVec.z <= 0.0f) ? 
        refractColor + f * reflectColor : reflectColor;
    result = vec3(
        pow(result.x,  0.4545f),
        pow(result.y,  0.4545f),
        pow(result.z,  0.4545f)
    );

// phong na mapingu
    //result = Phong(pos, N, viewVec, result); // ostatecznie nie korzystamy z phonga
    color = vec4(result, 1);
    

    //color = vec4(texture(heightMap, texCord).a, 1, 1,1) ;
}

