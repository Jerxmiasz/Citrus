#include <filesystem>
#include <fstream>
#include <string_view>

#include "Citrus/graphics/core/color.hpp"
#include "Citrus/sys/Vector2.hpp"
#include "Citrus/sys/Vector3.hpp"

namespace citrus::opengl {
  class ShaderProgram;

  class Shader {
   public:
    enum class ShaderType { VERTEX, FRAGMENT, GEOMETRY };
    Shader() = default;
    Shader(const std::filesystem::path& path, ShaderType type);
    Shader(std::istream& stream, ShaderType type);
    Shader(std::string_view source, ShaderType type);
    Shader& operator=(Shader&& other) noexcept {
      this->shader_handle_ = other.shader_handle_;
      other.shader_handle_ = 0;
      return *this;
    }
    bool fromFile(const std::filesystem::path& path, ShaderType type);
    bool fromFile(const std::istream& istream, ShaderType type);
    bool fromString(const std::string_view& source, ShaderType type);
    ~Shader();

   private:
    friend class ShaderProgram;
    unsigned int shader_handle_ = 0;
  };

  class ShaderProgram {
   public:
    static void setActiveProgramId(unsigned int id) {
      s_active_shader_program_handle_ = id;
    }
    static unsigned int getActiveProgramId() {
      return s_active_shader_program_handle_;
    }

    ShaderProgram() = default;
    ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
    ShaderProgram& operator=(const ShaderProgram&) = delete;
    ShaderProgram(ShaderProgram&& other) noexcept {
      this->shader_program_handle_ = other.shader_program_handle_;
      other.shader_program_handle_ = 0;
    }
    ShaderProgram& operator=(ShaderProgram&& other) noexcept {
      this->shader_program_handle_ = other.shader_program_handle_;
      other.shader_program_handle_ = 0;
      return *this;
    }
    bool operator==(const ShaderProgram& other) {
      return shader_program_handle_ == other.s_active_shader_program_handle_;
    }
    ~ShaderProgram();

    inline bool isActive() {
      return s_active_shader_program_handle_ == this->shader_program_handle_;
    }

    inline unsigned int getId() {
      return shader_program_handle_;
    }

    void setUniformVal(std::string_view uniform_name, Vector2f value);
    void setUniformVal(std::string_view uniform_name, Vector3f value);
    void setUniformVal(std::string_view uniform_name, Color value);
    void setUniformVal(std::string_view uniform_name, float value);
    void setUniformVal(std::string_view uniform_name, int value);

    void getUniformVal(std::string_view uniform_name, Vector2f& value);
    void getUniformVal(std::string_view uniform_name, Vector3f& value);
    void getUniformVal(std::string_view uniform_name, Color& value);
    void getUniformVal(std::string_view uniform_name, float& value);
    void getUniformVal(std::string_view uniform_name, int& value);

   private:
    static inline unsigned int s_active_shader_program_handle_ = 0;
    unsigned int shader_program_handle_ = 0;
  };

  inline constexpr unsigned int CitrusGlToGlShaderType(citrus::opengl::Shader::ShaderType type) {
    switch (type) {
      case citrus::opengl::Shader::ShaderType::VERTEX:
        return 0x8B31; 
      case citrus::opengl::Shader::ShaderType::FRAGMENT:
        return 0x8B30;
      case citrus::opengl::Shader::ShaderType::GEOMETRY:
        return 0x8DD9; 
    }
    return 0; 
  }

}