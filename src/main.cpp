#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

const int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

void processInput(GLFWwindow *window);

int main(void) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, 0);

  GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "[glad] GL with GLFW", nullptr, nullptr);

	if (!window) {
		std::cout << "Failed to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
  
	if (gladLoadGL(glfwGetProcAddress) == 0){
		std::cout << "Failed to load opengl function pointers" << std::endl;
		return -1;
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  while (!glfwWindowShouldClose(window)) {
		//Input
		processInput(window);
		
		//rendering
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

		//function calls and events
    glfwSwapBuffers(window);
		glfwPollEvents();
  }

  glfwTerminate();

  return 0;
}

void processInput(GLFWwindow *window){
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) ==  GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
