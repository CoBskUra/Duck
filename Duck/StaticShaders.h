#pragma once
#include "Shader.h"
#include <GLFW/glfw3.h>
#include "VAO.h"
#include "OpenGLHelper.h"
#include "Camera.h"
static struct StaticShaders{
	static void Init(); // u¿yj przed u¿yciem shaderów

	static Shader GetSimpleShader();
	static Shader GetPhongShader();
	static Shader GetDefaultShader();
	static Shader GetReflectionWorldPhongShader();
	static Shader GetDuckShader();
	static Shader GetWaterShader();
	static Shader GetRoomShader();

	static void MustHaveShaderInput(VAO vao, GLuint shaderId, const Camera& camera, 
		const Light* lights, int lightsCount, glm::mat4 modelMtx);

private:
	static bool haseBeenInit;
	static Shader simpleShader;
	static Shader defaultShader;
	static Shader phongShader;
	static Shader reflectionWorldPhongShader;
	static Shader duckShader;
	static Shader waterShader;
	static Shader roomShader;
};