#pragma once
#include "Camera.h"
#include "VAO.h"
#include "Light.h"
#include "Texture.h"
#include "StaticShaders.h"

class Room
{
	Shader shader = StaticShaders::GetPhongShader();
	VAO vao;
	glm::mat4 walls_modelMtx[6]{};
public:
	glm::mat4 modelMtx{ 1.0f }; 
	Texture cubeMap{
		{
		"./Textures/shreck/right.jpg",
		"./Textures/shreck/left.jpg",
		"./Textures/shreck/top.jpg",
		"./Textures/shreck/bottom.jpg",
		"./Textures/shreck/front.jpg",
		"./Textures/shreck/back.jpg"
		}
	};

	Room(float wdith, float height, float deep);
	void virtual Draw(GLFWwindow* window, const Camera& camera, const Light* lights, int lightsCount, glm::mat4 trans = glm::mat4{ 1.0f });
	void UserInterfers();

};

