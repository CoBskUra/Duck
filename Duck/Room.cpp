#include "Room.h"
#include "EBO.h"
#include "OpenGLHelper.h"



Room::Room(float width, float height, float deep)
{
	const float halfWidth = width * 0.5f;
	const float halfHeight = height * 0.5f;
	const float halfDeep = deep * 0.5f;

	// Location, Normal, Color
	std::vector<float> wallVs = { 
		-1, -1, 0, 0, 0, -1, 0.66f, 0.70f, 0.21f, -1, -1,
		-1, +1, 0, 0, 0, -1, 0.66f, 0.70f, 0.21f, -1, +1,
		+1, +1, 0, 0, 0, -1, 0.66f, 0.70f, 0.21f, +1, +1,
		+1, -1, 0, 0, 0, -1, 0.66f, 0.70f, 0.21f, +1, -1,
	};

	std::vector<GLuint> wallIes = {
		0, 1, 2,
		0, 2, 3
	};

	vao_wall.Bind();
	VBO vbo(wallVs, GL_STATIC_DRAW);
	EBO ebo(wallIes);

	vao_wall.LinkAttrib(0, 3, GL_FLOAT, false, 11 * sizeof(float), 0);
	vao_wall.LinkAttrib(1, 3, GL_FLOAT, false, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	vao_wall.LinkAttrib(2, 3, GL_FLOAT, false, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	vao_wall.LinkAttrib(3, 2, GL_FLOAT, false, 11 * sizeof(float), (void*)(9 * sizeof(float)));

	vao_wall.Unbind(); vbo.Unbind(); ebo.Unbind();

	shader = StaticShaders::GetPhongShader();

	float startPos_y = halfHeight - 1;
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.0, 0.0, -halfDeep));
	trans = glm::translate(trans, glm::vec3(0.0, startPos_y, 0.0));
	trans = glm::rotate(trans, glm::radians(180.0f), glm::vec3(1.0, 0.0, 0.0));
	trans = glm::rotate(trans, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(halfWidth, halfHeight, 1));
	walls_modelMtx[0] = trans;

	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.0, 0.0, +halfDeep));
	trans = glm::translate(trans, glm::vec3(0.0, startPos_y, 0.0));
	trans = glm::scale(trans, glm::vec3(halfWidth, halfHeight, 1));
	walls_modelMtx[1] = trans;

	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.0, -halfHeight, 0.0));
	trans = glm::translate(trans, glm::vec3(0.0, startPos_y, 0.0));
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
	trans = glm::scale(trans, glm::vec3(halfWidth, halfDeep, 1));
	walls_modelMtx[2] = trans;

	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.0, +halfHeight, 0.0));
	trans = glm::translate(trans, glm::vec3(0.0, startPos_y, 0.0));
	trans = glm::rotate(trans, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
	trans = glm::scale(trans, glm::vec3(halfWidth, halfDeep, 1));
	walls_modelMtx[3] = trans;

	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(-halfWidth, 0.0, 0.0));
	trans = glm::translate(trans, glm::vec3(0.0, startPos_y, 0.0));
	trans = glm::rotate(trans, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
	trans = glm::scale(trans, glm::vec3(halfDeep, halfHeight, 1));
	walls_modelMtx[4] = trans;

	trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(+halfWidth, 0.0, 0.0));
	trans = glm::translate(trans, glm::vec3(0.0, startPos_y, 0.0));
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
	trans = glm::scale(trans, glm::vec3(halfDeep, halfHeight, 1));
	walls_modelMtx[5] = trans;
}

void Room::Draw(GLFWwindow* window, const Camera& camera, const Light* lights, int lightsCount, glm::mat4 trans)

{
	shader.Activate();
	vao_wall.Bind();

	OpenGLHelper::loadLightUniform(shader.ID, lights, lightsCount, trans);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	// Camera location
	GLint viewPos = glGetUniformLocation(shader.ID, "viewPos");
	auto cameraPos = camera.GetPosition();
	glUniform3f(viewPos, cameraPos.x, cameraPos.y, cameraPos.z);

	for (int i = 0; i < sizeof(walls_modelMtx) / sizeof(walls_modelMtx[0]); i++) {

		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "MODEL_MATRIX"),
			1, GL_FALSE, glm::value_ptr(trans * walls_modelMtx[i]));
		camera.SaveMatrixToShader(shader.ID);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	vao_wall.Unbind();

	glDisable(GL_CULL_FACE);
}

void Room::UserInterfers()
{
}
