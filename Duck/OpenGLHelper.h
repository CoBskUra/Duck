#pragma once
#include <glm/ext/vector_float3.hpp>
#include <vector>
#include "Light.h"
#include <GLFW/glcorearb.h>

static class OpenGLHelper {
public:
	static void AddVecToVector(std::vector<float>& vector, const glm::vec3& pos) {
		vector.push_back(pos.x); vector.push_back(pos.y); vector.push_back(pos.z);
	}

	static void AddVecToVector(std::vector<GLuint>& vector, const glm::uvec3& pos) {
		vector.push_back(pos.x); vector.push_back(pos.y); vector.push_back(pos.z);
	}

	static void AddVecToVector(std::vector<float>& vector, const glm::vec4& pos) {
		vector.push_back(pos.x); vector.push_back(pos.y); vector.push_back(pos.z); vector.push_back(pos.w);
	}

	static glm::vec3 TakeLastVecFromVector(std::vector<float>& vector) {
		glm::vec3 pos{ vector.at(vector.size() - 3), vector.at(vector.size() - 2), vector.at(vector.size() - 1) };
		vector.pop_back();
		vector.pop_back();
		vector.pop_back();

		return pos;
	}

	static glm::vec3 TakeCopyOfVec3(std::vector<float>& vector, int id, int offset, int size) {
		glm::vec3 pos{
			vector.at(id * size + offset),
			vector.at(id * size + 1 + offset), 
			vector.at(id * size + 2 + offset) };
		return pos;
	}

	template<typename T>
	static T TakeOneCopyOf(std::vector<T>& vector, int id, int offset, int size) {
		return vector.at(id * size + offset);
	}

	static void loadLightUniform(GLuint shaderProgram, const Light* lights, int lightCount, glm::mat4 trans = glm::mat4{1.0f}) {
		for (int i = 0; i < lightCount; i++) {
			std::string uniformName = "lights[" + std::to_string(i) + "]";
			// Get the location of the uniform variables
			GLint posLoc = glGetUniformLocation(shaderProgram, (uniformName + ".position").c_str());
			GLint ambLoc = glGetUniformLocation(shaderProgram, (uniformName + ".ambient").c_str());
			GLint diffLoc = glGetUniformLocation(shaderProgram, (uniformName + ".diffuse").c_str());
			GLint specLoc = glGetUniformLocation(shaderProgram, (uniformName + ".specular").c_str());
			// Set the values of the uniform variables
			glm::vec4 posTmp = trans * glm::vec4(lights[i].position, 1);
			posTmp /= posTmp.w;
			glUniform3f(posLoc, posTmp.x, posTmp.y, posTmp.z);
			glUniform3f(ambLoc, lights[i].ambient.x, lights[i].ambient.y, lights[i].ambient.z);
			glUniform3f(diffLoc, lights[i].diffuse.x, lights[i].diffuse.y, lights[i].diffuse.z);
			glUniform3f(specLoc, lights[i].specular.x, lights[i].specular.y, lights[i].specular.z);
		}
		// Set the number of lights
		GLint numLightsLoc = glGetUniformLocation(shaderProgram, "numLights");
		glUniform1i(numLightsLoc, lightCount);
	}
};