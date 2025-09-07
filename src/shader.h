#ifndef SHADER_H
#define SHADER_H

#include "glad/gl.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

class Shader {
public:
  // the program ID
  unsigned int ID;

  // constructor reads and builds the Shader
  Shader(const char *vertexPath, const char *fragmentPath);
  // use/activate the Shader
  void Use();
  // utility uniform functions
  void SetBool(const std::string &name, bool value) const;
  void SetInt(const std::string &name, int value) const;
  void SetFloat(const std::string &name, float value) const;
};
#endif
