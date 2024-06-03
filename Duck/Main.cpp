#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include "Libraries/include/ImGui/imgui.h"
#include "Libraries/include/ImGui/imgui_impl_glfw.h"
#include "Libraries/include/ImGui/imgui_impl_opengl3.h"
#include "Camera.h"
#include "VAO.h"
#include "StaticShaders.h"
#include"Duck.h"
#include "Room.h"
#include "Water.h"

unsigned int width = 1600;
unsigned int height = 1024;
const char* name = "DUCK";
const int RoomWidth = 5, RoomDeep = 5;
const float duckDisturbeValue = 5;
const int maxWaterDrops = 6;
const float maxDisturbeValue = 5;
const float waterLevel = -1.2;

Camera camera(width, height, glm::vec3(0.0f, 0.0f, -5.0f));
GLFWwindow* Init();
void ResizeCallBack(GLFWwindow* window, int w, int h);

int main()
{
	GLFWwindow* window = Init();
	if (window == NULL)
		return -1;

	float width = 30;
	float height = 10;
	float deep = 30;


	const int lightCount = 1;
	Light lights[lightCount];


	Light light;
	light.position = glm::vec3(0.0f, 2.0f, 0);
	light.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	light.diffuse = glm::vec3(.4f, 0.5f, 0.5f);
	light.specular = glm::vec3(0.5f, 0.5f, 0.5f);

	lights[0] = light;

	//Light light2;
	//light2.position = glm::vec3(0.0f, 8.0f, 0);
	//light2.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	//light2.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	//light2.specular = glm::vec3(0.5f, 1.0f, 0.5f);

	//lights[1] = light2;
	
	Room room{ RoomWidth, 5, RoomDeep };
	Duck duck{ RoomWidth, RoomDeep };
	Water water{ RoomWidth, waterLevel, RoomDeep };
	water.roomModelMtx = &room.modelMtx;
	water.roomTexture = &room.cubeMap;
	duck.watterLevel = waterLevel;

	////////////////////////////////
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		

		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		camera.Inputs(window);
		camera.ActiveInterferes();

		room.Draw(window, camera, lights, lightCount);
		duck.Draw(window, camera, lights, lightCount);
		glm::vec3 pos = duck.GetDuckPos();
		water.Disturb({ pos.x / (float) RoomWidth, pos.z / (float)RoomDeep }, duckDisturbeValue);
		int waterDrops = rand() % maxWaterDrops;
		for (int i = 0; i < waterDrops; i++) {
			water.Disturb(
				{
					2.0f * rand() / (float)RAND_MAX - 1.0f,
					2.0f * rand() / (float)RAND_MAX - 1.0f 
				},
				rand() / (float)RAND_MAX * maxDisturbeValue);
		}

		water.Draw(window, camera, lights, lightCount);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Delete all the objects we've created

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}


GLFWwindow* Init() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, name, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, ResizeCallBack);

	gladLoadGL();
	glViewport(0, 0, width, height);


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	StaticShaders::Init();

	return window;
}

void ResizeCallBack(GLFWwindow* window, int w, int h)
{
	width = w;
	height = h;
	glViewport(0, 0, width, height);
	camera.SetAspect(width / (float)height);
}
