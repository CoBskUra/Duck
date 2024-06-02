#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nom;
layout (location = 2) in vec2 tex;

// Outputs the color for the Fragment Shader
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

// Imports the camera matrix from the main function
uniform mat4 MODEL_MATRIX;
uniform mat4 CAM_MATRIX;

void main()
{
	// Outputs the positions/coordinates of all vertices
	// CAM_MATRIX *
	FragPos =  (MODEL_MATRIX *  vec4(pos, 1.0)).rgb;
	gl_Position = CAM_MATRIX *vec4(FragPos, 1);
	TexCoord = tex;

	
    Normal = mat3(transpose(inverse(MODEL_MATRIX))) * nom;
}