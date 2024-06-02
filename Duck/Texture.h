#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>

#include"Shader.h"

class Texture
{
public:
	GLuint ID;
	GLenum type;
	Texture(GLenum texType);
	Texture(std::vector<std::string> faces);
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	unsigned int loadCubemap(std::vector<std::string> faces);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();

	void Recreat();

	~Texture();
};
#endif