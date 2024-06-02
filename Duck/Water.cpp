#include "Water.h"
#include "EBO.h"

void Water::SaveHeightMap(Texture& tex)
{
	// Tworzenie dwuwymiarowej tablicy z danymi pikseli (RGBA)
	std::vector<float> data(N * N * 4); // 4 kana³y (RGBA)
	for (int y = 1; y < N - 1; y++) {
		for (int x = 1; x < N - 1; x++) {
			float heightL = waterHeights_current[y][x - 1];
			float heightR = waterHeights_current[y][x + 1];
			float heightD = waterHeights_current[y - 1][x];
			float heightU = waterHeights_current[y + 1][x];

			glm::vec3 dx = glm::normalize(glm::vec3(1.0f, heightR - heightL, 0.0f));
			glm::vec3 dz = glm::normalize(glm::vec3(0.0f, heightU - heightD, 1.0));
			glm::vec3 normal = glm::normalize(glm::cross(dz, dx));

			normal = normal*0.5f + 0.5f;
			int index = (y * N + x) * 4;
			data[index + 0] = normal.x;	// R
			data[index + 1] = normal.y; // waterHeights_current[y][x];//rand()/(float) RAND_MAX; // G
			data[index + 2] = normal.z;	// B
			data[index + 3] = waterHeights_current[y][x] / codeWaterHeight;	// A
		}
	}

	tex.Recreat();
	tex.Bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, N, N, 0, GL_RGBA, GL_FLOAT, data.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	tex.Unbind();
}

void Water::NextSimulationStep()
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			float current_h = waterHeights_current[i][j];
			float last_h = waterHeights_last[i][j];

			float neighbours_h = 0;
			if (i > 0)
				neighbours_h += waterHeights_current[i - 1][j];
			if (i < N - 1)
				neighbours_h += waterHeights_current[i + 1][j];

			if (j > 0)
				neighbours_h += waterHeights_current[i][j - 1];
			if (j < N - 1)
				neighbours_h += waterHeights_current[i][j + 1];

			waterHeights_last[i][j] = dumping[i][j] * (A * neighbours_h + B * current_h - last_h);
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			float tmp = waterHeights_last[i][j];
			waterHeights_last[i][j] = waterHeights_current[i][j];
			waterHeights_current[i][j] = tmp;
		}
	}
}

Water::Water(float width, float height, float deep): width{width}, waterLevel{ height }, deep{ deep}
{
	waterHeights_current = std::vector<std::vector<float>>(N);
	waterHeights_last = std::vector<std::vector<float>>(N);
	dumping = std::vector<std::vector<float>>(N);
	for (int i = 0; i < N; i++) {
		waterHeights_current[i] = std::vector<float>(N);
		waterHeights_last[i] = std::vector<float>(N);
		dumping[i] = std::vector<float>(N);

		for (int j = 0; j < N; j++) {
			float l = fmin(i, N - i - 1);
			l = fmin(fmin(l, j), N - 1 - j);
			l = l / (float)N * 2.0f;
			dumping[i][j] = 0.95f * fmin(1, l / 0.2f);
		}
	}

	// Location, Normal, Color
	std::vector<float> wallVs = {
		-1, 0, -1,	 0, 1, 0,	+0, +0,
		-1, 0, +1,	 0, 1, 0,	+0, +1,
		+1, 0, +1,	 0, 1, 0,	+1, +1,
		+1, 0, -1,	 0, 1, 0,	+1, +0,
	};

	std::vector<GLuint> wallIes = {
		0, 1, 2,
		0, 2, 3
	};

	vao.Bind();
	VBO vbo(wallVs, GL_STATIC_DRAW);
	EBO ebo(wallIes);

	vao.LinkAttrib(0, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	vao.LinkAttrib(1, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	vao.LinkAttrib(2, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	vao.Unbind(); vbo.Unbind(); ebo.Unbind();

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.0, this->waterLevel, 0.0));
	trans = glm::scale(trans, glm::vec3(this->width * 0.5f, 1, this->deep * 0.5f));
	modelMtx = trans;

	SaveHeightMap(normalsTexture);


}

void Water::Draw(GLFWwindow* window, const Camera& camera, const Light* lights, int lightsCount, glm::mat4 trans)
{
	NextSimulationStep();
	SaveHeightMap(normalsTexture);

	shader.Activate();
	normalsTexture.texUnit(shader, "heightMap", 0);
	normalsTexture.Bind();
	roomTexture->Bind();
	roomTexture->texUnit(shader, "cubMap", 0);
	vao.Bind();
	{

		OpenGLHelper::loadLightUniform(shader.ID, lights, lightsCount, trans);
		GLint viewPos = glGetUniformLocation(shader.ID, "viewPos");
		auto cameraPos = camera.GetPosition();
		glUniform3f(viewPos, cameraPos.x, cameraPos.y, cameraPos.z);

		camera.SaveMatrixToShader(shader.ID);
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "MODEL_MATRIX"),
			1, GL_FALSE, glm::value_ptr(trans * modelMtx));

		glUniform1f(glGetUniformLocation(shader.ID, "maxWaterHeight"),
			maxWaterHeight);

		glUniform1f(glGetUniformLocation(shader.ID, "codeWaterHeight"),
			codeWaterHeight);

		glUniform1f(glGetUniformLocation(shader.ID, "waterLevel"),
			waterLevel);

		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "MODEL_MATRIX"),
			1, GL_FALSE, glm::value_ptr(trans * modelMtx));

		glDrawElements(GL_TRIANGLES, iesNum, GL_UNSIGNED_INT, 0);
	}
	vao.Unbind();
	normalsTexture.Unbind();
	roomTexture->Unbind();

}

void Water::UserInterfers()
{

}

void Water::Disturb(glm::vec2 pos, float value)
{
	//float i = pos.y /(float) width;
	//float j = pos.x / (float)deep;

	float i = pos.y;
	float j = pos.x;

	if (abs(i) > 0.5f || abs(j) > 0.5f)
		return;

	i =  (i + 0.5f) * (N - 1);
	j = (j + 0.5f) * (N - 1);

	//waterHeights_last[i][j] += 0.25f;
	waterHeights_current[i][j] += value;
}
