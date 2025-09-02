#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>
#include <cmath>
#include "imgui.h"

const int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

void processInput(GLFWwindow *window);

// vertex shader code
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aColor;\n"
                                 "out vec3 ourColor;\n"
                                 "void main() {\n"
                                 "gl_Position = vec4(aPos, 1.0);\n"
                                 "ourColor = aColor;\n"
                                 "}\n";

// fragment shader code
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec3 ourColor;\n"
                                   "void main() {\n"
                                   "FragColor = vec4(ourColor, 1.0f);\n"
                                   "}\n";

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

  float firstTraingle[] = {
      // positios 					// colors
      -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
      0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top
  };

  int success;
  char infoLog[512];

  // VERTEX SHADER
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER); // create vertex shader
  glShaderSource(vertexShader, 1, &vertexShaderSource,
                 nullptr); // link shader code to the shader
  glCompileShader(vertexShader);

  // check if shader compilation failed
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cout << "ERROR: VERTEX SHADER COMPILATION FAILED\n"
              << infoLog << std::endl;
  }

  // FRAGMENT SHADER [ORANGE]
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // create fragment shader
  glShaderSource(fragmentShader, 1, &fragmentShaderSource,
                 nullptr); // link fragment shader code to the shader.
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    std::cout << "ERROR: ORANGE FRAGMENT SHADER COMPILATION FAILED\n"
              << infoLog << std::endl;
  }

  // Shader program
  unsigned int shaderProgram = glCreateProgram(); // create shader program

  // Attaching and linking shaderProgram
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    std::cout << "ERROR: LINKING SHADERS FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // VERTEX BUFFER, VERTEX ATTRIBUTE...
  unsigned int vertexBufferObject, vertexArrayObject, elementBufferObject;

  glGenBuffers(1, &vertexBufferObject);     // create the vertex buffer object
  glGenVertexArrays(1, &vertexArrayObject); // create the vertex array object

  // FOR FIRST TRIANGLE
  glBindVertexArray(vertexArrayObject); // bind the vertex array object
  glBindBuffer(GL_ARRAY_BUFFER,
               vertexBufferObject); // 'bind' the buffer to memory
  glBufferData(
      GL_ARRAY_BUFFER, sizeof(firstTraingle), firstTraingle,
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

	glUseProgram(shaderProgram); //WE'RE CALLING THIS OUT OF THE GAME LOOP SINCE ITS JUST A SINGLE SHADER(PROGRAM) WE HAVE
  while (!glfwWindowShouldClose(window)) {
    // Input
    processInput(window);

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // first triangle
    glBindVertexArray(vertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // function calls and events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vertexArrayObject);
  glDeleteBuffers(1, &vertexBufferObject);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
