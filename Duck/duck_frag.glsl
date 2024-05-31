#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;


// Inputs the color from the Vertex Shader
in vec2 TexCoord;

uniform sampler2D duckSkin;

void main()
{
    FragColor = texture(duckSkin, TexCoord);
	//FragColor = vec4(1, 1, 1, 1);
}