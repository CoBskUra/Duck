#pragma once
#include "Camera.h"
#include "VAO.h"
#include "Light.h"
#include "Robot.h"

class Room
{
	Shader shader = StaticShaders::GetPhongShader();
	VAO vao_wall;
	glm::mat4 walls_modelMtx[6]{};
	glm::vec4 wall_colors[6] = { 
		{0, 0.5, 0, 1}, 
		{0.5, 0.5, 0, 1},
		{0, 0.5, 0.5, 1},
		{0.5, 0, 0.5, 1},
		{0.5, 0, 0, 1},
		{0, 0, 0.5, 1}
	};
public:
	Room(float wdith, float height, float deep);
	void virtual Draw(GLFWwindow* window, const Camera& camera, const Light* lights, int lightsCount, glm::mat4 trans = glm::mat4{ 1.0f });
	void UserInterfers();

};

