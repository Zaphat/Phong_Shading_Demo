#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/shader.h"
#include "graphics/model.h"
#include "graphics/models/cube.hpp"
#include "graphics/models/lamp.hpp"
#include "graphics/models/sphere.hpp"
#include "graphics/models/pyramid.hpp"
#include "io/keyboard.h"
#include "io/mouse.h"
#include "io/screen.h"
#include "io/camera.h"


void processInput(double deltaTime);

Screen screen;
glm::mat3 mouseTransform = glm::mat4(1.0f);
Camera Camera::defaultCamera(glm::vec3(0.0f,2.0f, 5.0f));
double deltaTime = 0.0f; 
double lastFrame = 0.0f; 

int main() {

	std::cout << "Hello, OpenGL!" << std::endl;
	glfwInit();

	// openGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	# ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COPMPAT, GL_TRUE);
	#endif
	//catch errors
	if (!screen.init()) {
		std::cout << "Could not open window" << std::endl;
		glfwTerminate();
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	screen.setParameters();

	// SHADERS
	Shader shader("assets/object.vs", "assets/object.fs");
	Shader lampShader("assets/object.vs", "assets/lamp.fs");
	// MODELS
	Cube cube(Material::cyan_plastic, glm::vec3(1.66f, -2.28f, 0.0f), glm::vec3(0.75f));
	Pyramid pyramid(Material::gold, glm::vec3(-1.9f, -1.2f,0.0f ), glm::vec3(1.5f));
	Sphere sphere(Material::obsidian, glm::vec3(0.89f, 1.95f, 0.0f), glm::vec3(0.8f));
	cube.init();
	pyramid.init();
	sphere.init();
	Lamp lamp(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.25f));
	lamp.init();

	while (!screen.shouldClose()) {
		// calculate deltaTime
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;
		// process input
		processInput(deltaTime);
		// render
		screen.update();
		shader.activate();

		shader.set3Float("light.position", lamp.pos);
		shader.set3Float("viewPos", Camera::defaultCamera.cameraPos);

		// set light values
		shader.set3Float("light.ambient", lamp.ambient);
		shader.set3Float("light.diffuse", lamp.diffuse);
		shader.set3Float("light.specular", lamp.specular);
		
		// create transformation
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = Camera::defaultCamera.getViewMatrix();
		projection = glm::perspective(
			glm::radians(Camera::defaultCamera.zoom),
			(float)Screen::SCR_WIDTH / (float)Screen::SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		cube.render(shader);
		pyramid.render(shader);
		sphere.render(shader);

		lampShader.activate();
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		lamp.render(lampShader);
		// send new frame to window
		screen.newFrame();
		glfwPollEvents();
	}

	cube.cleanup();
	pyramid.cleanup();
	sphere.cleanup();
	lamp.cleanup();
	glfwTerminate();
	return 0;
}
void processInput(double deltaTime) {
	if (Keyboard::key(GLFW_KEY_ESCAPE)) {
		screen.setShouldClose(true);
	}
	// move camera
	if (Keyboard::key(GLFW_KEY_SPACE)) {
		Camera::defaultCamera.updateCameraPos(CameraDirection::FORWARD, deltaTime);
	}
	if (Keyboard::key(GLFW_KEY_LEFT_SHIFT)) {
		Camera::defaultCamera.updateCameraPos(CameraDirection::BACKWARD, deltaTime);
	}
	if (Keyboard::key(GLFW_KEY_D)) {
		Camera::defaultCamera.updateCameraPos(CameraDirection::RIGHT, deltaTime);
	}
	if (Keyboard::key(GLFW_KEY_A)) {
		Camera::defaultCamera.updateCameraPos(CameraDirection::LEFT, deltaTime);
	}
	if (Keyboard::key(GLFW_KEY_W)) {
		Camera::defaultCamera.updateCameraPos(CameraDirection::UP, deltaTime);
	}
	if (Keyboard::key(GLFW_KEY_S)) {
		Camera::defaultCamera.updateCameraPos(CameraDirection::DOWN, deltaTime);
	}
}