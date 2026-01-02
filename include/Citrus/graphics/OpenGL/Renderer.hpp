#ifndef CITRUS_GRAPHICS_OPENGLRENDERER_HPP
#define CITRUS_GRAPHICS_OPENGLRENDERER_HPP

#include <fstream>
#include <queue>
#include <filesystem>

#include "Citrus/sys/sys.hpp"
#include "Citrus/graphics/core/Vertex.hpp"
#include "Shader.hpp"


namespace citrus::opengl {

  using PreDrawFunc =  void(*)(ShaderProgram& shader_program);

  struct DrawBatch {
    std::vector<VertexBuffer> vertex_buffers;
    ShaderProgram* shader_program;
    PreDrawFunc pre_draw_func;
  };

  class Renderer {
    public:
    
    Renderer() = delete;

    explicit Renderer(const Window& window);

    ~Renderer();

    ShaderProgram& getGenericShaderProgram() {
      return generic_shader_program_;
    }

    void useShaderProgram(ShaderProgram & program);

    void draw(const VertexBuffer& buf, ShaderProgram& shader_program,  PreDrawFunc func);
    void draw(DrawBatch&& batch, PreDrawFunc pre_draw_func);

    void clearColor(Color color);
    void present(); // Shows every change to the screen

    private:
    unsigned int vbo_ = 0, vao_ = 0, ebo_ = 0;
    std::queue<DrawBatch> draw_batch_queue_;
    size_t allocated_vertex_count_ = 0;
    const Window* window_ = nullptr;
    ShaderProgram generic_shader_program_; // Generic shader program shall be used for simple 2d draw operations;
  };
  
}

#endif