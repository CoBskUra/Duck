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
#include "Texture.h"

class Duck 
{
	Texture texture;
	VAO vao;
	Shader shader = StaticShaders::GetDuckShader();
	int iesNum;
	int vertNum;
	const int RoomWidth, RoomDeep;
	float journeyTime = 1;
	glm::vec3 b0{ 0,0,0 }, b1{ 0,0,0 }, b2{ 0,0,0 }, b3{ 0,0,0 };
	glm::vec3 e{ 0,0,0 }, f{ 0,0,0 }, g{ 0,0,0 }, e_next{ 0,0,0 };
	float startTime = 0;
	glm::vec3 duckPos;


	//VAO vao_road;
	//Shader shaderRoad = StaticShaders::GetSimpleShader();
	glm::vec3 NewDuckPos(float time, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	glm::vec3 Derirative(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	void NewDeborePoints();
	void CalculateDezierPoint();
	//void CalculateBezierCurve();
public:
	float watterLevel = 0;


	float scale = 0.005;
	Duck(int RoomWidth, int RoomDeep);
	void virtual Draw(GLFWwindow* window, const Camera& camera, const Light* lights, int lightsCount, glm::mat4 trans = glm::mat4{ 1.0f });
	void UserInterfers();
	glm::vec3 GetDuckPos() const;
};
