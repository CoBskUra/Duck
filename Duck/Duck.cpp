#include "Duck.h"

glm::vec3 Duck::NewDuckPos(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	//glm::vec3 p0 = (b0 + 4.0f * b1 + b2) / 6.0f;
	//glm::vec3 p1 = (2.0f * b1 + b2) / 3.0f;
	//glm::vec3 p2 = (b1 + 2.0f * b2) / 3.0f;
	//glm::vec3 p3 = (b1 + 4.0f * b2 + b3) / 6.0f;

	p0 = p0 * (1 - t) + p1 * t;
	p1 = p1 * (1 - t) + p2 * t;
	p2 = p2 * (1 - t) + p3 * t;

	p0 = p0 * (1 - t) + p1 * t;
	p1 = p1 * (1 - t) + p2 * t;


	p0 = p0 * (1 - t) + p1 * t;

	return p0;
}

glm::vec3 Duck::Derirative(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
	p0 = -3.0f * p0 + 3.0f * p1;
	p1 = -3.0f * p1 + 3.0f * p2;
	p2 = -3.0f * p2 + 3.0f * p3;

	p0 = p0 * (1 - t) + p1 * t;
	p1 = p1 * (1 - t) + p2 * t;

	p0 = p0 * (1 - t) + p1 * t;;

	return p0;
}

void Duck::NewDeborePoints()
{
	b0 = b1;
	b1 = b2;
	b2 = b3;
	b3 = glm::vec3{ MathOperations::Random(-RoomWidth * 0.5f, RoomWidth * 0.5f), 0, MathOperations::Random(-RoomDeep * 0.5f, RoomDeep * 0.5f) } ;
	
	
	CalculateBezierCurve();
}

void Duck::CalculateDezierPoint() {
	e = e_next;
	f = (2.0f * b0 + 1.0f * b1) / 3.0f;
	g = (1.0f * b0 + 2.0f * b1) / 3.0f;
	e_next = g + (2.0f * b1 + 1.0f * b2) / 3.0f;
	e_next /= 2.0f;
}

void Duck::CalculateBezierCurve()
{
	std::vector<float> vs;
	OpenGLHelper::AddVecToVector(vs, b0);
	OpenGLHelper::AddVecToVector(vs, b1);
	OpenGLHelper::AddVecToVector(vs, b2);
	OpenGLHelper::AddVecToVector(vs, b3);

	vao_road.Bind();
	VBO vbo(vs, GL_DYNAMIC_DRAW);
	vao_road.LinkAttrib(0, 3, GL_FLOAT, false, 3 * sizeof(float), 0);
	vao_road.Unbind(); vbo.Unbind();
}

Duck::Duck(int RoomWidth, int RoomDeep) : RoomWidth{RoomWidth}, RoomDeep{RoomDeep}, texture{"./duck_texMesh/ducktex.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE}
{

    std::ifstream file("./duck_texMesh/duck.txt");

    if (!file.is_open()) {
        std::cerr << "Nie mo¿na otworzyæ pliku!" << std::endl;
        exit(1);
    }


	vertNum = AszklarMeshReader::ReadOne<int>(file);
	std::vector<float> vsNormTex = AszklarMeshReader::ReadLines<float>(vertNum, file);

	iesNum = AszklarMeshReader::ReadOne<int>(file);
	std::vector<GLuint> ies = AszklarMeshReader::ReadLines<GLuint>(iesNum, file);
	iesNum *= 3;

	file.close();


	vao.Bind();
	VBO vbo(vsNormTex, GL_STATIC_DRAW);
	EBO ebo(ies);

	vao.LinkAttrib(0, 3, GL_FLOAT, false, 8 * sizeof(float), 0); // position
	vao.LinkAttrib(1, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float))); // normals
	vao.LinkAttrib(2, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float))); // tex

	vao.Unbind(); vbo.Unbind(); ebo.Unbind();


	NewDeborePoints();
	startTime = glfwGetTime();
	b0 = glm::vec3{ 0, 0, 0 };
	e = b0;
	f = b1;
	g = (b1 + b2) * 0.5f;
	glm::vec3 f_next = (2.0f * b2 + 1.0f * b3) / 3.0f;
	e_next = (g + f_next) * 0.5f;
	NewDeborePoints();
	//modelMtx = glm::scale(modelMtx, glm::vec3{ 1, 1, 1 } * 0.005f);

}

void Duck::Draw(GLFWwindow* window, const Camera& camera, const Light* lights, int lightsCount, glm::mat4 trans)
{
	float t = (glfwGetTime() - startTime) / journeyTime;
	if (t > 1) {
		startTime = glfwGetTime();
		NewDeborePoints();
		CalculateDezierPoint();
		t = 0;
	}
	duckPos = NewDuckPos(t, e, f, g, e_next);
	duckPos.y = watterLevel;

	glm::vec3 div = Derirative(t, e, f, g, e_next);
	div.y = 0;


	float alfa = MathOperations::Angle(glm::vec3{ -1, 0, 0 }, div);
	if (div.z < 0) {
		alfa = -alfa;
	}


	glm::mat4 modelMtx{ 1.0f };
	modelMtx = glm::translate(modelMtx, duckPos);
	modelMtx = glm::rotate(modelMtx, alfa, glm::vec3{ 0, 1, 0 });
	modelMtx = glm::scale(modelMtx, glm::vec3{ 1, 1, 1 } * scale);

	shader.Activate();
	texture.texUnit(shader, "duckSkin", 0);
	texture.Bind();
	vao.Bind(); 
	
	{
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "MODEL_MATRIX"),
			1, GL_FALSE, glm::value_ptr(trans * modelMtx));
		camera.SaveMatrixToShader(shader.ID);

		glDrawElements(GL_TRIANGLES, iesNum, GL_UNSIGNED_INT, 0);
	}
	vao.Unbind();
	texture.Unbind();

	vao_road.Bind(); 
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderRoad.ID, "MODEL_MATRIX"),
			1, GL_FALSE, glm::value_ptr(trans));
		camera.SaveMatrixToShader(shaderRoad.ID);
		glUniform4f(glGetUniformLocation(shaderRoad.ID, "COLOR"),
			1, 1, 1, 1);

		glDrawArrays(GL_LINE_STRIP, 0, 4);
	}
	vao_road.Unbind();

}

void Duck::UserInterfers()
{
}

glm::vec3 Duck::GetDuckPos() const
{
	return duckPos;
}
