#include "StaticShaders.h"

bool StaticShaders::haseBeenInit = false;

Shader StaticShaders::simpleShader;
Shader StaticShaders::defaultShader;
Shader StaticShaders::phongShader;
Shader StaticShaders::reflectionWorldPhongShader;
Shader StaticShaders::duckShader;
Shader StaticShaders::waterShader;
Shader StaticShaders::roomShader;

Shader StaticShaders::GetSimpleShader() {
	return simpleShader;
}

Shader StaticShaders::GetDefaultShader() {
	return defaultShader;
}

Shader StaticShaders::GetPhongShader()
{
	return phongShader;
}


Shader StaticShaders::GetReflectionWorldPhongShader()
{
	return reflectionWorldPhongShader;
}

Shader StaticShaders::GetDuckShader()
{
	return duckShader;
}

Shader StaticShaders::GetWaterShader()
{
	return waterShader;
}

Shader StaticShaders::GetRoomShader()
{
	return roomShader;
}

void StaticShaders::MustHaveShaderInput(VAO vao, GLuint shaderId, const Camera& camera, const Light* lights, int lightsCount, glm::mat4 modelMtx)
{
	vao.Bind();
	{
		OpenGLHelper::loadLightUniform(shaderId, lights, lightsCount);

		// Camera location
		GLint viewPos = glGetUniformLocation(shaderId, "viewPos");
		auto cameraPos = camera.GetPosition();
		glUniform3f(viewPos, cameraPos.x, cameraPos.y, cameraPos.z);
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "MODEL_MATRIX"),
			1, GL_FALSE, glm::value_ptr(modelMtx));
		camera.SaveMatrixToShader(shaderId);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	vao.Unbind();
}

void StaticShaders::Init()
{
	if (haseBeenInit)
		return;
	haseBeenInit = true;

	simpleShader = { "simple_vert.glsl", "simple_frag.glsl" };
	defaultShader = { "default.vert", "default.frag" };
	phongShader = { "phong_vert.glsl", "phong_frag.glsl" };
	reflectionWorldPhongShader = { "reflectionWordPhong_vert.glsl", "reflectionWordPhong_frag.glsl" };
	duckShader = { "duck_vert.glsl", "duck_frag.glsl" };
	waterShader = { "water_vert.glsl", "water_frag.glsl" };
	roomShader = { "room_vert.glsl", "room_frag.glsl" };
}