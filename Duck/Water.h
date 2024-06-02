#pragma once
#include <ImGui/imgui_impl_glfw.h>
#include "Camera.h"
#include "Light.h"
#include "VAO.h"
#include "StaticShaders.h"
#include "Texture.h"
class Water
{
	VAO vao;
	Shader shader = StaticShaders::GetWaterShader();
	float waterLevel;
	float width;
	float deep;
	const GLuint iesNum{6};

	std::vector<std::vector<float>> waterHeights_current;
	std::vector<std::vector<float>> waterHeights_last;
	std::vector<std::vector<float>> damping;

	Texture normalsTexture{GL_TEXTURE_2D};
	void SaveHeightMap(Texture& tex);
	void NextSimulationStep();
	const float codeWaterHeight = 128.0f;
	const float maxWaterHeight = 100.0f;
public:
	const int N = 256;
	const float h = 2.0f / (float)(N - 1);
	const float c = 1.0f;

	const float delta = 1.0f / (float)N;
	const float A = powf(c * delta * (N - 1) / 2.0f, 2);
	const float B = 2.0f * (1 - 2.0f * A);

	glm::mat4 modelMtx{ 1.0f };
	Texture *roomTexture;
	glm::mat4 *roomModelMtx;

	Water(float width, float height, float deep);
	void virtual Draw(GLFWwindow* window, const Camera& camera, const Light* lights, int lightsCount, glm::mat4 trans = glm::mat4{ 1.0f });
	void UserInterfers();

	// pos -1 - maxLeft 1 - max right
	void Disturb(glm::vec2 pos, float value);
};

