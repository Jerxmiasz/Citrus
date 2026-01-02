#include <span>
#include <stdexcept>
#include <stddef.h>
#include <utility>

#include "Citrus/graphics/OpenGL/Renderer.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

constexpr auto generic_vertex_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "void main()"
    "{"
    "gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
    "}";
constexpr auto generic_fragment_shader_source =
    "#version 330 core\n"
    "uniform vec4 in_color;\n"
    "out vec4 FragColor;\n"
    "void main()"
    "{"
    "FragColor = in_color;\n"
    "}";


namespace citrus::opengl {

  Renderer::Renderer(const Window& window) {
    window_ = &window;
    glfwMakeContextCurrent(window.getGlfwPtr());
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
      throw std::runtime_error(
          "Failed to initialize OpenGL functions, your device may not be "
          "compatible with OpenGL");
    }
    glViewport(0, 0, window.getSize().x, window.getSize().y);

    {
      auto generic_vertex_shader =
          Shader(std::string_view(generic_vertex_shader_source),
               Shader::ShaderType::VERTEX);
      auto generic_fragment_shader =
          Shader(std::string_view(generic_fragment_shader_source),
               Shader::ShaderType::FRAGMENT);
      generic_shader_program_ =
          ShaderProgram(generic_vertex_shader, generic_fragment_shader);
    }

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

    // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }
  // TODO: Make DrawBatch hold an array of different Pre-Draw functions
  void Renderer::draw(const VertexBuffer& vertices, ShaderProgram& shader_program, PreDrawFunc pre_draw_func) {
    if (this->draw_batch_queue_.empty() || this->draw_batch_queue_.back().shader_program->getId() != shader_program.getId()) {
      DrawBatch new_batch;
      new_batch.vertex_buffers.emplace_back(vertices.getVertices());
      new_batch.shader_program = std::addressof(shader_program);
      new_batch.pre_draw_func = pre_draw_func;
      this->draw_batch_queue_.push(std::move(new_batch));
      return;
    }
    this->draw_batch_queue_.back().vertex_buffers.emplace_back(vertices.getVertices());
  }

  void Renderer::present() { 
    while (!this->draw_batch_queue_.empty()) {
      DrawBatch batch = std::move(draw_batch_queue_.front());
      draw_batch_queue_.pop();

      this->useShaderProgram(*batch.shader_program);

      std::vector<Vertex> vertices;
      vertices.reserve(this->allocated_vertex_count_);

      for (const VertexBuffer& vertex_buffer : batch.vertex_buffers) {
        for (const Vertex& vertex : vertex_buffer.getVertices()) {
          vertices.emplace_back(vertex);
        }
      }

      if (batch.pre_draw_func) {
        batch.pre_draw_func(*batch.shader_program);
      }
      glBindVertexArray(vao_);
      glBindBuffer(GL_ARRAY_BUFFER, vbo_);
      
      if (vertices.size() > this->allocated_vertex_count_) {
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
        this->allocated_vertex_count_ = vertices.size();
      } else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
      }
      glDrawArrays(GL_TRIANGLES, 0, vertices.size());
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    }
    glfwSwapBuffers(window_->getGlfwPtr()); 
  }

  Renderer::~Renderer() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
  }
  void Renderer::useShaderProgram( ShaderProgram & program) {
    if (!program.isActive()) {
      glUseProgram(program.getId());
      ShaderProgram::setActiveProgramId(program.getId());
    }
  }
  void Renderer::clearColor(Color color) {
    auto color_arr = color.asFloatRgba();
    glClearColor(color_arr[0], color_arr[1], color_arr[2], color_arr[3]);
    glClear(GL_COLOR_BUFFER_BIT);
  }
};
