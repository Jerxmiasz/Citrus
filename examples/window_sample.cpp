#include "Citrus/graphics/OpenGL/Renderer.hpp"
#include <cstdio>

void* operator new(size_t sz) {
  std::printf("Bytes allocated: %lld\n",sz);
  return malloc(sz);
}

int main() {
  auto window = citrus::Window("Test Window", {300,300});
  auto renderer = citrus::opengl::Renderer(window);
  auto& generic_shader_program = renderer.getGenericShaderProgram();
  // Used for plain 2d images
  auto square_vertices = citrus::VertexBuffer({ // Triangle 1
    citrus::Vertex(citrus::Color(1.0f, 0.0f, 0.0f, 1.0f), citrus::Vector3f(-0.5f, 0.5f, 0.0f)),
    citrus::Vertex(citrus::Color(0.0f, 1.0f, 0.0f, 1.0f), citrus::Vector3f(0.5f, 0.5f, 0.0f)),
    citrus::Vertex(citrus::Color(0.0f, 0.0f, 1.0f, 1.0f), citrus::Vector3f(0.5f, -0.5f, 0.0f)),
    // Triangle 2
    citrus::Vertex(citrus::Color(0.0f, 0.0f, 1.0f, 1.0f), citrus::Vector3f(0.5f, -0.5f, 0.0f)),
    citrus::Vertex(citrus::Color(1.0f, 1.0f, 0.0f, 1.0f), citrus::Vector3f(-0.5f, -0.5f, 0.0f)),
    citrus::Vertex(citrus::Color(1.0f, 0.0f, 0.0f, 1.0f), citrus::Vector3f(-0.5f, 0.5f, 0.0f))
  });
  std::puts("Prepared vertices");
  while (window.isOpen()) {
    while (auto event = window.getEvent()) {
      if (event->matches<citrus::Window::Event::Closed>()) {
        window.close();
        break;
      }
    }
    renderer.clearColor(citrus::Color::FromByteRGBA(40,160,240));
    renderer.draw(square_vertices, generic_shader_program, [](citrus::opengl::ShaderProgram& program){
      program.setUniformVal("in_color", citrus::Color::FromByteRGBA(255,0,0));
    });
    std::puts("Draw called");
    renderer.present();
    std::puts("Presented queue to gpu");
  }
  return 0;
}