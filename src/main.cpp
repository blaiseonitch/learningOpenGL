#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <ostream>
#include <cmath>
#include "imgui.h"

const int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

void processInput(GLFWwindow *window);

// vertex shader code
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec4 vertexColor;\n"
    "void main() {\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);\n"
    "}";

// fragment shader code
const char *fragmentShaderSourceOrange =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec4 vertexColor;\n"
    "uniform vec4 newColor;\n"
    "void main() {\n"
    // "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "FragColor = newColor;\n"
    "}";

const char *fragmentShaderSourceYellow =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}";

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
      -0.9f,  -0.5f, 0.0f, // bottom left
      0.0f,   -0.5f, 0.0f, // bottom right
      -0.45f, 0.5f,  0.0f, // top center
  };

  float secondTraingle[] = {
      0.0f,  -0.5f, 0.0f, // bottom left
      0.9f,  -0.5f, 0.0f, // bottom right
      0.45f, 0.5f,  0.0f  // top center
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
  unsigned int fragmentShaderOrange;
  fragmentShaderOrange =
      glCreateShader(GL_FRAGMENT_SHADER); // create fragment shader
  glShaderSource(fragmentShaderOrange, 1, &fragmentShaderSourceOrange,
                 nullptr); // link fragment shader code to the shader.
  glCompileShader(fragmentShaderOrange);

  glGetShaderiv(fragmentShaderOrange, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShaderOrange, 512, nullptr, infoLog);
    std::cout << "ERROR: ORANGE FRAGMENT SHADER COMPILATION FAILED\n"
              << infoLog << std::endl;
  }

  // FRAGMENT SHADER [YELLOW]
  unsigned int fragmentShaderYellow;
  fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceYellow, nullptr);
  glCompileShader(fragmentShaderYellow);
  glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShaderYellow, 512, nullptr, infoLog);
    std::cout << "ERROR: YELLOW FRAGMENT SHADER COMPILATION FAILED\n"
              << infoLog << std::endl;
  }

  // Shader program
  unsigned int shaderProgramOrange, shaderProgramYellow;
  shaderProgramOrange = glCreateProgram(); // create shader program
  shaderProgramYellow =
      glCreateProgram(); // shader program for our yellow triangle

  // Attaching and linking orange shaderProgram
  glAttachShader(shaderProgramOrange, vertexShader);
  glAttachShader(shaderProgramOrange, fragmentShaderOrange);
  glLinkProgram(shaderProgramOrange);

  glGetProgramiv(shaderProgramOrange, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgramOrange, 512, nullptr, infoLog);
    std::cout << "ERROR: LINKING SHADERS FAILED\n" << infoLog << std::endl;
  }

  // Attaching and linking yellow shaderProgram
  glAttachShader(shaderProgramYellow, vertexShader);
  glAttachShader(shaderProgramYellow, fragmentShaderYellow);
  glLinkProgram(shaderProgramYellow);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShaderOrange);
  glDeleteShader(fragmentShaderYellow);

  // VERTEX BUFFER, VERTEX ATTRIBUTE...
  unsigned int vertexBufferObject[2], vertexArrayObject[2], elementBufferObject;

  glGenBuffers(2, vertexBufferObject);     // create the vertex buffer object
  glGenVertexArrays(2, vertexArrayObject); // create the vertex array object

  // FOR FIRST TRIANGLE
  glBindVertexArray(vertexArrayObject[0]); // bind the vertex array object
  glBindBuffer(GL_ARRAY_BUFFER,
               vertexBufferObject[0]); // 'bind' the buffer to memory
  glBufferData(
      GL_ARRAY_BUFFER, sizeof(firstTraingle), firstTraingle,
      GL_STATIC_DRAW); // copy vertices data to the vertex buffer created

  // how OpenGL should interprete our vertex attribute data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // // FOR SECOND TRIANGLE
  glBindVertexArray(vertexArrayObject[1]); // bind the vertex array object
  glBindBuffer(GL_ARRAY_BUFFER,
               vertexBufferObject[1]); // 'bind' the buffer to memory
  glBufferData(
      GL_ARRAY_BUFFER, sizeof(secondTraingle), secondTraingle,
      GL_STATIC_DRAW); // copy vertices data to the vertex buffer created

  // how OpenGL should interprete our vertex attribute data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  // uncomment this call to draw in wireframe polygons.
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while (!glfwWindowShouldClose(window)) {
    // Input
    processInput(window);

    // rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // first triangle
    glUseProgram(shaderProgramOrange);
    float timeValue = glfwGetTime();
    float greenValue =
        (sin(timeValue) / 2.0f) * 0.5f; // random values using a sin function.
    int vertextColorLocation = glGetUniformLocation(
        shaderProgramOrange,
        "newColor"); // get the uniform from the shader program (which has the
                     // shader containing the declaration of the uniform)
    glUniform4f(vertextColorLocation, 0.0f, greenValue, 0.0f,
                1.0f); // updating the uniform's value

    glBindVertexArray(vertexArrayObject[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // glUseProgram(shaderProgramYellow);
    // // second triangle
    // glBindVertexArray(vertexArrayObject[1]);
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    // function calls and events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(2, vertexArrayObject);
  glDeleteBuffers(2, vertexBufferObject);
  glDeleteProgram(shaderProgramOrange);
  glDeleteProgram(shaderProgramYellow);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
