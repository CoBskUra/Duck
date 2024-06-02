#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;


out vec3 FragPos;
out vec3 Normal;
out vec3 texCord;

uniform mat4 MODEL_MATRIX;
uniform mat4 CAM_MATRIX;

void main()
{
    FragPos = vec3(MODEL_MATRIX * vec4(aPos, 1.0));
    Normal =  mat3(transpose(inverse(MODEL_MATRIX))) * aNormal;
    gl_Position = CAM_MATRIX * vec4(FragPos, 1.0);
    texCord = aPos;
}