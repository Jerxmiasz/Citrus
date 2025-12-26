#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Citrus/sys/Window.hpp"

int main() {
  auto window = citrus::Window("Hello world", {300, 300});
  glfwMakeContextCurrent(window.getGlfwPtr());
  gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
  while (window.isOpen()) {
    while(auto evt = window.getEvent()) {
      if (evt->matches<citrus::Window::Event::Closed>()){
        window.close();
      };
      if (evt->matches<citrus::Window::Event::KeyDown>()) {
        auto down_evt = evt->get<citrus::Window::Event::KeyDown>();
        std::cout << static_cast<char>(down_evt.key) ;
      }
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.3f, 0.4f, 0.5f, 1);
    glfwSwapBuffers(window.getGlfwPtr());
  }
}