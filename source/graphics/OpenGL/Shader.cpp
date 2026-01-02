#include "glad/glad.h"
#include "Citrus/graphics/OpenGL/Shader.hpp"

namespace citrus::opengl {
  bool Shader::fromString(const std::string_view& source, ShaderType type) {
    this->shader_handle_ = glCreateShader(CitrusGlToGlShaderType(type));
    const char* src = source.data();
    glShaderSource(this->shader_handle_, 1, &src, nullptr);
    glCompileShader(this->shader_handle_);
    int success;
    glGetShaderiv(this->shader_handle_, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      int info_log_size = 0;
      glGetShaderiv(this->shader_handle_, GL_INFO_LOG_LENGTH, &info_log_size);
      auto info_log = std::string(info_log_size, '\0');
      glGetShaderInfoLog(this->shader_handle_, 512, nullptr, &info_log[0]);
      throw std::runtime_error(info_log);
    }
    return true;
  }
  bool Shader::fromFile(const std::filesystem::path& path, ShaderType type) {
    std::ifstream file(path);
    if (!file.is_open()) {
      throw std::runtime_error("Could not open shader file with name: " + path.string());
    }
    auto source = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return this->fromString(source, type);
  }
    
  Shader::Shader(std::string_view source, ShaderType type) {
    this->fromString(source, type);
  };
  Shader::Shader(const std::filesystem::path& path, ShaderType type) {
    this->fromFile(path, type);
  }
  Shader::Shader(std::istream& stream, ShaderType type) {
    auto source = std::string((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
    this->fromString(source, type);
  }
  Shader::~Shader() {
    glDeleteShader(this->shader_handle_);
  }
  ShaderProgram::ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader) {
    this->shader_program_handle_ = glCreateProgram();
    glAttachShader(this->shader_program_handle_, vertexShader.shader_handle_);
    glAttachShader(this->shader_program_handle_, fragmentShader.shader_handle_);
    glLinkProgram(this->shader_program_handle_);
    int success;
    glGetProgramiv(this->shader_program_handle_, GL_LINK_STATUS, &success);
    if (!success)
    {
      int info_log_size = 0;
      glGetProgramiv(this->shader_program_handle_, GL_INFO_LOG_LENGTH, &info_log_size);
      auto info_log = std::string(info_log_size, '\0');
      glGetProgramInfoLog(this->shader_program_handle_, info_log_size, nullptr, &info_log[0]);
      throw std::runtime_error(info_log);
    }
  }
  ShaderProgram::~ShaderProgram() {
    glDeleteProgram(this->shader_program_handle_);
  }
  inline bool EnsureShaderProgramIsActive(ShaderProgram& program) {
    if (!program.isActive()) {
      throw std::runtime_error("Attempted to get uniform from a shader program which is not active");
    }
    return true;
  }
  void ShaderProgram::setUniformVal(std::string_view uniform_name, Vector2f value) {
    EnsureShaderProgramIsActive(*this);
    glUniform2f(glGetUniformLocation(this->shader_program_handle_, uniform_name.data()), value.x, value.y);
  }
  void ShaderProgram::setUniformVal(std::string_view uniform_name, Vector3f value) {
    EnsureShaderProgramIsActive(*this);
    glUniform3f(glGetUniformLocation(this->shader_program_handle_, uniform_name.data()), value.x, value.y, value.z);
  }
  void ShaderProgram::setUniformVal(std::string_view uniform_name, Color value) {
    EnsureShaderProgramIsActive(*this);
    auto color_arr = value.asFloatRgba();
    glUniform4f(glGetUniformLocation(this->shader_program_handle_, uniform_name.data()), color_arr[0], color_arr[1], color_arr[2], color_arr[3]);
  }
  void ShaderProgram::setUniformVal(std::string_view uniform_name, float value) {
    EnsureShaderProgramIsActive(*this);
    glUniform1f(glGetUniformLocation(this->shader_program_handle_, uniform_name.data()), value);
  }
  void ShaderProgram::setUniformVal(std::string_view uniform_name, int value) {
    EnsureShaderProgramIsActive(*this);
    glUniform1i(glGetUniformLocation(this->shader_program_handle_, uniform_name.data()), value);
  }
  void ShaderProgram::getUniformVal(std::string_view uniform_name, Vector2f& value) {
    EnsureShaderProgramIsActive(*this);
    GLfloat params[2];
    glGetUniformfv(this->shader_program_handle_, glGetUniformLocation(this->shader_program_handle_, uniform_name.data()), params);
    value.x = params[0];
    value.y = params[1];
  }
  void ShaderProgram::getUniformVal(std::string_view uniform_name, Vector3f& value) {
    EnsureShaderProgramIsActive(*this);
    GLfloat params[3];
    glGetUniformfv(this->shader_program_handle_, glGetUniformLocation(this->shader_program_handle_, uniform_name.data()), params);
    value.x = params[0];
    value.y = params[1];
    value.z = params[2];
  }
  void ShaderProgram::getUniformVal(std::string_view uniform_name, Color& value) {
    EnsureShaderProgramIsActive(*this);
    GLfloat params[4];
    glGetUniformfv(this->shader_program_handle_, glGetUniformLocation(this->shader_program_handle_, uniform_name.data()), params);
    value = Color(params[0], params[1], params[2], params[3]);
  }
  void ShaderProgram::getUniformVal(std::string_view uniform_name, float& value) {
    EnsureShaderProgramIsActive(*this);
    glGetUniformfv(this->shader_program_handle_, glGetUniformLocation(this->shader_program_handle_, uniform_name.data()), &value);
  }
  void ShaderProgram::getUniformVal(std::string_view uniform_name, int& value) {
    EnsureShaderProgramIsActive(*this);
    glGetUniformiv(this->shader_program_handle_, glGetUniformLocation(this->shader_program_handle_, uniform_name.data()), &value);
  }

}