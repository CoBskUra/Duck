#include "Room.h"
#include "EBO.h"
#include "OpenGLHelper.h"



Room::Room(float width, float height, float deep)
{
	const float halfWidth = width * 0.5f;
	const float halfHeight = height * 0.5f;
	const float halfDeep = deep * 0.5f;

	// positons


	// Location, Normal, Color
	std::vector<float> wallVs = {
		// positions			//norm			//color
		-1.0f,  1.0f, -1.0f,	+0, +0, +1, 	0.5f, 0.8f, 0.8f,
		-1.0f, -1.0f, -1.0f,	+0, +0,	+1, 	0.5f, 0.3f, 0.8f,
		 1.0f, -1.0f, -1.0f,	+0, +0,	+1, 	0.5f, 0.3f, 0.8f,
		 1.0f, -1.0f, -1.0f,	+0, +0,	+1, 	0.5f, 0.5f, 0.8f,
		 1.0f,  1.0f, -1.0f,	+0, +0,	+1, 	0.3f, 0.3f, 0.8f,
		-1.0f,  1.0f, -1.0f,	+0, +0,	+1, 	0.5f, 0.3f, 0.8f,

		-1.0f, -1.0f,  1.0f,	+1, +0, +0,		0.5f, 0.3f, 0.8f,
		-1.0f, -1.0f, -1.0f,	+1, +0, +0,		0.5f, 0.3f, 0.8f,
		-1.0f,  1.0f, -1.0f,	+1, +0, +0,		0.5f, 0.3f, 0.8f,
		-1.0f,  1.0f, -1.0f,	+1, +0, +0,		0.5f, 0.3f, 0.8f,
		-1.0f,  1.0f,  1.0f,	+1, +0, +0,		0.5f, 0.3f, 0.8f,
		-1.0f, -1.0f,  1.0f,	+1, +0, +0,		0.5f, 0.3f, 0.8f,

		 1.0f, -1.0f, -1.0f,	-1, +0, +0,		0.5f, 0.3f, 0.8f,
		 1.0f, -1.0f,  1.0f,	-1, +0, +0,		0.5f, 0.3f, 0.8f,
		 1.0f,  1.0f,  1.0f,	-1, +0, +0,		0.5f, 0.3f, 0.8f,
		 1.0f,  1.0f,  1.0f,	-1, +0, +0,		0.5f, 0.3f, 0.8f,
		 1.0f,  1.0f, -1.0f,	-1, +0, +0,		0.5f, 0.3f, 0.8f,
		 1.0f, -1.0f, -1.0f,	-1, +0, +0,		0.5f, 0.3f, 0.8f,

		-1.0f, -1.0f,  1.0f,	+0, +0, -1,		0.5f, 0.3f, 0.8f,
		-1.0f,  1.0f,  1.0f,	+0, +0, -1,		0.5f, 0.3f, 0.8f,
		 1.0f,  1.0f,  1.0f,	+0, +0, -1,		0.5f, 0.3f, 0.8f,
		 1.0f,  1.0f,  1.0f,	+0, +0, -1,		0.5f, 0.3f, 0.8f,
		 1.0f, -1.0f,  1.0f,	+0, +0, -1,		0.5f, 0.3f, 0.8f,
		-1.0f, -1.0f,  1.0f,	+0, +0, -1,		0.5f, 0.3f, 0.8f,

		-1.0f,  1.0f, -1.0f,	+0, -1, +0,		0.5f, 0.3f, 0.8f,
		 1.0f,  1.0f, -1.0f,	+0, -1, +0,		0.5f, 0.3f, 0.8f,
		 1.0f,  1.0f,  1.0f,	+0, -1, +0,		0.5f, 0.3f, 0.8f,
		 1.0f,  1.0f,  1.0f,	+0, -1, +0,		0.5f, 0.3f, 0.8f,
		-1.0f,  1.0f,  1.0f,	+0, -1, +0,		0.5f, 0.3f, 0.8f,
		-1.0f,  1.0f, -1.0f,	+0, -1, +0,		0.5f, 0.3f, 0.8f,

		-1.0f, -1.0f, -1.0f,	+0, +1, +0,		0.5f, 0.3f, 0.8f,
		-1.0f, -1.0f,  1.0f,	+0, +1, +0,		0.5f, 0.3f, 0.8f,
		 1.0f, -1.0f, -1.0f,	+0, +1, +0,		0.5f, 0.3f, 0.8f,
		 1.0f, -1.0f, -1.0f,	+0, +1, +0,		0.5f, 0.3f, 0.8f,
		-1.0f, -1.0f,  1.0f,	+0, +1, +0,		0.5f, 0.3f, 0.8f,
		 1.0f, -1.0f,  1.0f,	+0, +1, +0,		0.5f, 0.3f, 0.8f
	};

	vao.Bind();
	VBO vbo(wallVs, GL_STATIC_DRAW);

	vao.LinkAttrib(0, 3, GL_FLOAT, false, 9 * sizeof(float), 0);
	vao.LinkAttrib(1, 3, GL_FLOAT, false, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	vao.LinkAttrib(2, 3, GL_FLOAT, false, 9 * sizeof(float), (void*)(6 * sizeof(float)));

	vao.Unbind(); vbo.Unbind();

	shader = StaticShaders::GetRoomShader();


	modelMtx = glm::mat4(1.0f);
	modelMtx = glm::translate(modelMtx, glm::vec3(0.0f, 0.0f, 0.0f));
	modelMtx = glm::scale(modelMtx, glm::vec3(halfWidth, halfHeight, halfDeep));

	

}

void Room::Draw(GLFWwindow* window, const Camera& camera, const Light* lights, int lightsCount, glm::mat4 trans)

{
	shader.Activate();
	vao.Bind();
	glActiveTexture(GL_TEXTURE0);

	cubeMap.Bind();
	cubeMap.texUnit(shader, "cubMap", GL_TEXTURE0);

	OpenGLHelper::loadLightUniform(shader.ID, lights, lightsCount, trans);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	// Camera location
	GLint viewPos = glGetUniformLocation(shader.ID, "viewPos");
	auto cameraPos = camera.GetPosition();
	glUniform3f(viewPos, cameraPos.x, cameraPos.y, cameraPos.z);


	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "MODEL_MATRIX"),
		1, GL_FALSE, glm::value_ptr(trans * modelMtx));
	camera.SaveMatrixToShader(shader.ID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	cubeMap.Unbind();
	vao.Unbind();

	glDisable(GL_CULL_FACE);
}

void Room::UserInterfers()
{
}
