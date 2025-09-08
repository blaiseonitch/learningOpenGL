#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <iostream>
#include <ostream>
#include <cmath>
#include "imgui.h"
#include "stb_image.h"

const int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

void processInput(GLFWwindow *window);

int main(void) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, 0);

  GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT,
                                        "LearnOpenGL", nullptr, nullptr);

  if (!window) {
    std::cout << "Failed to create glfw window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (gladLoadGL(glfwGetProcAddress) == 0) {
    std::cout << "Failed to load opengl function pointers" << std::endl;
    return -1;
  }

  float triangle[] = {
      // positios 					// colors
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
      0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top
  };

  Shader firstShader("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");

  float offset = 0.5f;

  // VERTEX BUFFER, VERTEX ATTRIBUTE...
  unsigned int vertexBufferObject, vertexArrayObject, elementBufferObject;

  glGenBuffers(1, &vertexBufferObject);     // create the vertex buffer object
  glGenVertexArrays(1, &vertexArrayObject); // create the vertex array object

  // FOR FIRST TRIANGLE
  glBindVertexArray(vertexArrayObject); // bind the vertex array object
  glBindBuffer(GL_ARRAY_BUFFER,
               vertexBufferObject); // 'bind' the buffer to memory
  glBufferData(
      GL_ARRAY_BUFFER, sizeof(triangle), triangle,
      GL_STATIC_DRAW); // copy vertices data to the vertex buffer created

  // how OpenGL should interprete our vertex (positions) attribute data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // how OpenGL should interprete our vertex (colors) attribute data
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  // uncomment this call to draw in wireframe polygons.
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // ITS JUST A SINGLE SHADER(PROGRAM) WE HAVE
  while (!glfwWindowShouldClose(window)) {
    // Input
    processInput(window);

    // rendering
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    firstShader.Use(); // equevalent to glUseProgram(firstShader);
    firstShader.SetFloat("xOffset", offset); // apply value to our uniform

    // first triangle
    glBindVertexArray(vertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // function calls and events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vertexArrayObject);
  glDeleteBuffers(1, &vertexBufferObject);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
