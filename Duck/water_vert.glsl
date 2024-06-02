#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 tex;

out vec3 localPos;
out vec3 FragPos;
out vec3 Normal;
out vec2 texCord;


uniform mat4 MODEL_MATRIX;
uniform mat4 CAM_MATRIX;
uniform float waterLevel;

void main()
{
    localPos = aPos;
    localPos.y = waterLevel;

    FragPos = vec3(MODEL_MATRIX * vec4(aPos, 1.0));
    gl_Position = CAM_MATRIX * vec4(FragPos, 1.0);

    texCord = tex;

    Normal = mat3(transpose(inverse(MODEL_MATRIX))) * aNormal;
}