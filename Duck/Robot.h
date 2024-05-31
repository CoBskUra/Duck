#pragma once
#include "Camera.h"
#include "AszkalMeshReader.h"
#include "OpenGLHelper.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Light.h"
#include "Parametryzatrons.h"
#include "StaticShaders.h"

class Robot 
{
	const int numberOfParts = 6;

	glm::vec3 partsStartPositions[6]{
		{0, 0, 0},
		{0, 0, 0},
		{0, .27, 0},
		{-.91, .27, 0},
		{-2.05, .27, -.26},
		{-1.72, .27, 0}
	};

	glm::vec3 partsRotationAxis[6]{
		{0, 0, 0},
		{0, 1, 0},
		{0, 0, 1},
		{0, 0, 1},
		{1, 0, 0},
		{0, 0, 1}
	};

	RobotsPartInfo partsInfo[6];

	glm::mat4 partsModel_Mtx[6];
	float angels[6]{0.0f};

	VAO partsVAO_triangles[6];
	VAO partsVAO_edges[6];

	Shader edgesShader;
	Shader shader;

	glm::vec3 robotColor = { 0.85, 0.85, 0.85 };

	void UpdateRobotPosition(); 
	void inverse_kinematics(glm::vec3 pos, glm::vec3 normal, float& a1, float& a2, float& a3, float& a4, float& a5);

public:
	Robot();
	void virtual Draw(GLFWwindow* window, const Camera& camera, const Light* lights, int lightsCount, glm::mat4 trans = glm::mat4{ 1.0f });
	void UserInterfers();
	void inverse_kinematics(glm::vec3 pos, glm::vec3 normal);

};

