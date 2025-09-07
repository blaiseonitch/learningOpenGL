#include "shader.h"
#include "glad/gl.h"
#include "iostream"
#include <sstream>
#include <string>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  // retrieve the vertex/fragment source code from filepath
  std::string vertexCode;
  std::string fragmentCode;
  std::ifstream vertexShaderFile;
  std::ifstream fragmentShaderFile;

  vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    // open files
    vertexShaderFile.open(vertexPath);
    fragmentShaderFile.open(fragmentPath);
    std::stringstream vertexShaderStream, fragmentShaderStream;
    // read file content into sstreams
    vertexShaderStream << vertexShaderFile.rdbuf();
    fragmentShaderStream << fragmentShaderFile.rdbuf();
    // close file handlers
    vertexShaderFile.close();
    fragmentShaderFile.close();
    // convert streams into string
    vertexCode = vertexShaderStream.str();
    fragmentCode = fragmentShaderStream.str();
  } catch (std::ifstream::failure e) {
    std::cout << "ERROR::SHADER FILE NOT SUCCESSFULLY READ" << std::endl;
  } 

  const char *vertexShaderCode = vertexCode.c_str();
  const char *fragmentShaderCode = fragmentCode.c_str();

  // compiling shaders
  unsigned int vertexShader, fragmentShader;
  int success;
  const int infoBufferSize = 512;
  char infoLog[infoBufferSize];

  // compiling vertex shader
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
  glCompileShader(vertexShader);

  // check if compilation was successful
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, infoBufferSize, nullptr, infoLog);
    std::cout << "ERROR: VERTEX SHADER COMPILATION FAILED" << infoLog
              << std::endl;
  }
  // compiling fragment shader
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
  glCompileShader(fragmentShader);

  // check if compilation was successful
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, infoBufferSize, nullptr, infoLog);
    std::cout << "ERROR: FRAGMENT SHADER COMPILATION FAILED" << infoLog
              << std::endl;
  }

  // create and link shader program to ID
  ID = glCreateProgram();
  glAttachShader(ID, vertexShader);
  glAttachShader(ID, fragmentShader);
  glLinkProgram(ID);

  // check for linking errors
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, infoBufferSize, nullptr, infoLog);
    std::cout << "ERROR: SHADER PROGRAM LINKING FAILED" << infoLog << std::endl;
  }

  // delete shaders after successful linking
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
};

// function declarations
void Shader::Use() { glUseProgram(ID); }

void Shader::SetBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
