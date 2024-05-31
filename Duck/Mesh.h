#pragma once
#include "Camera.h"
class Mesh
{
public:
	void virtual Draw(GLFWwindow* window, const Camera& camera);
	Mesh();
};

