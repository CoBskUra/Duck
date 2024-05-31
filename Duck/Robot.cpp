#include "Robot.h"

void Robot::UpdateRobotPosition()
{
	//partsModel_Mtx[0] = glm::mat4{ 1.0f };
	for (int i = 1; i < numberOfParts; i++)
	{
		glm::mat4 trans = glm::translate(partsModel_Mtx[i - 1], partsStartPositions[i]);
		trans = glm::rotate(trans, angels[i], partsRotationAxis[i]);
		trans = glm::translate(trans, -partsStartPositions[i]);
		partsModel_Mtx[i] = trans;
	}

}

void Robot::inverse_kinematics(glm::vec3 pos, glm::vec3 normal, float& a1, float& a2,
	float& a3, float& a4, float& a5)
{
	float l1 = .91f, l2 = .81f, l3 = .33f, dy = .27f, dz = .26f;
	normal = glm::normalize(normal);

	glm::vec3 pos1 = pos + normal * l3;
	float e = sqrtf(pos1.z * pos1.z + pos1.x * pos1.x - dz * dz);
	a1 = atan2(pos1.z, -pos1.x) + atan2(dz, e);

	glm::vec3 pos2(e, pos1.y - dy, .0f);
	a3 = -acosf(fmin(1.0f, (pos2.x * pos2.x + pos2.y * pos2.y - l1 * l1 - l2 * l2)
		/ (2.0f * l1 * l2)));

	float k = l1 + l2 * cosf(a3), l = l2 * sinf(a3);
	a2 = -atan2(pos2.y, sqrtf(pos2.x * pos2.x + pos2.z * pos2.z)) - atan2(l, k);

	glm::vec3 normal1;
	normal1 = glm::vec3(glm::rotate(glm::mat4{1.0f}, -a1, glm::vec3{0, 1, 0}) *
		glm::vec4(normal.x, normal.y, normal.z, .0f));
	normal1 = glm::vec3(glm::rotate(glm::mat4{ 1.0f }, -(a2 + a3), glm::vec3{ 0, 0, 1 }) *
		glm::vec4(normal1.x, normal1.y, normal1.z, .0f));
	a5 = acosf(normal1.x);

	a4 = atan2(normal1.z, normal1.y);
}



void Robot::Draw(GLFWwindow* window, const Camera& camera, const Light* lights, int lightsCount, glm::mat4 trans)
{
	UpdateRobotPosition();
	shader.Activate();
	
	// Camera location
	GLint viewPos = glGetUniformLocation(shader.ID, "viewPos");
	auto cameraPos = camera.GetPosition();
	glUniform3f(viewPos, cameraPos.x, cameraPos.y, cameraPos.z);
	glUniform1f(glGetUniformLocation(shader.ID, "alfa"), 1);

	OpenGLHelper::loadLightUniform(shader.ID, lights, lightsCount, trans);
	
	for (int i = 0; i < 6; i++) {
		partsVAO_triangles[i].Bind(); 
		{

			glUniformMatrix4fv(glGetUniformLocation(shader.ID, "MODEL_MATRIX"),
				1, GL_FALSE, glm::value_ptr(trans * partsModel_Mtx[i]));
			camera.SaveMatrixToShader(shader.ID);

			int n = partsInfo[i].triangles.size();
			glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0);
		}
		partsVAO_triangles[i].Unbind();
	}

	// mo¿e siê przydaæ do debugowania
	/*edgesShader.Activate();
	for (int i = 0; i < 6; i++) {
		partsVAO_edges[i].Bind();
		{
			glUniformMatrix4fv(glGetUniformLocation(edgesShader.ID, "MODEL_MATRIX"),
				1, GL_FALSE, glm::value_ptr(partsModel_Mtx[i]));
			camera.SaveMatrixToShader(edgesShader.ID);

			int n = partsInfo[i].edges.size() / 2;
			glDrawElements(GL_LINES, n, GL_UNSIGNED_INT, 0);
		}
		partsVAO_edges[i].Unbind();
	}*/
}

void Robot::UserInterfers()
{
	ImGui::BeginGroup();
	{
		ImGui::SliderAngle("a1", &angels[1]);
		ImGui::SliderAngle("a2", &angels[2]);
		ImGui::SliderAngle("a3", &angels[3]);
		ImGui::SliderAngle("a4", &angels[4]);
		ImGui::SliderAngle("a5", &angels[5]);
	}
	ImGui::EndGroup();
}

void Robot::inverse_kinematics(glm::vec3 pos, glm::vec3 normal)
{
	inverse_kinematics(pos, normal, angels[1], angels[2], angels[3], angels[4], angels[5]);
}

Robot::Robot()
{
	const char* meshSource[6]{
		".\\puma-mesh\\mesh1.txt",
		".\\puma-mesh\\mesh2.txt",
		".\\puma-mesh\\mesh3.txt",
		".\\puma-mesh\\mesh4.txt",
		".\\puma-mesh\\mesh5.txt",
		".\\puma-mesh\\mesh6.txt"
	};

	for (int i = 0; i < 6; i++) {
		partsInfo[i] = AszklarMeshReader::Read(meshSource[i]);
		partsModel_Mtx[i] = glm::mat4{ 1.0f };

		//////////////// triangle ///////////////////
		{
			std::vector<float> vertexNormalsColors;
			int n = partsInfo[i].vertexId_And_normals.size() / partsInfo[i].vertexId_And_normals_numberOfFloatsInDataUnit;

			for (int j = 0; j < n; j++) {
				int vertexPosId = OpenGLHelper::TakeOneCopyOf<float>(partsInfo[i].vertexId_And_normals,
					j, 0, partsInfo[i].vertexId_And_normals_numberOfFloatsInDataUnit);
				glm::vec3 vertexPos = OpenGLHelper::TakeCopyOfVec3(partsInfo[i].vertexPositions,
					vertexPosId, 0, partsInfo[i].vertexPositions_numberOfFloatsInDataUnit);
				OpenGLHelper::AddVecToVector(vertexNormalsColors, vertexPos);

				glm::vec3 normal = OpenGLHelper::TakeCopyOfVec3(partsInfo[i].vertexId_And_normals,
					j, 1, partsInfo[i].vertexId_And_normals_numberOfFloatsInDataUnit);
				OpenGLHelper::AddVecToVector(vertexNormalsColors, normal);

				OpenGLHelper::AddVecToVector(vertexNormalsColors, robotColor);
			}

			partsVAO_triangles[i].Bind();
			VBO vbo(vertexNormalsColors, GL_STATIC_DRAW);
			EBO ebo(partsInfo[i].triangles);

			partsVAO_triangles[i].LinkAttrib(0, 3, GL_FLOAT, false, 9 * sizeof(float), 0); // position
			partsVAO_triangles[i].LinkAttrib(1, 3, GL_FLOAT, false, 9 * sizeof(float), (void*)(3 * sizeof(float))); // normals
			partsVAO_triangles[i].LinkAttrib(2, 3, GL_FLOAT, false, 9 * sizeof(float), (void*)(6 * sizeof(float))); // color

			partsVAO_triangles[i].Unbind(); vbo.Unbind(); ebo.Unbind();
		}
		//////////////////////// edge ///////////
		
		// mo¿e siê przydaæ do debugowania
		
		//{
		//	std::vector<GLuint> edgesId;

		//	int n = partsInfo[i].edges.size() / partsInfo[i].edges_numberOfFloatsInDataUnit;
		//	for (int j = 0; j < n; j++) {
		//		int vertexPosId = OpenGLHelper::TakeOneCopyOf<GLuint>(partsInfo[i].edges,
		//			j, 0, partsInfo[i].edges_numberOfFloatsInDataUnit);

		//		edgesId.push_back(vertexPosId);

		//		vertexPosId = OpenGLHelper::TakeOneCopyOf<GLuint>(partsInfo[i].edges,
		//			j, 1, partsInfo[i].edges_numberOfFloatsInDataUnit);

		//		edgesId.push_back(vertexPosId);
		//	}

		//	partsVAO_edges[i].Bind();
		//	VBO vbo(partsInfo[i].vertexPositions, GL_STATIC_DRAW);
		//	EBO ebo(edgesId);

		//	partsVAO_edges[i].LinkAttrib(0, 3, GL_FLOAT, false, 3 * sizeof(float), 0); // position

		//	partsVAO_edges[i].Unbind(); vbo.Unbind(); ebo.Unbind();
		//}
	}

	shader = StaticShaders::GetPhongShader();
	edgesShader = StaticShaders::GetDefaultShader();
}
