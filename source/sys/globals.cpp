#include <format>

#include "Citrus/sys/Globals.hpp"
#include "glfw/glfw3.h"

namespace citrus {
  _Citrus_Sys_Context::_Citrus_Sys_Context() {
    glfwSetErrorCallback([](int code, const char* message){
      auto format_str = std::format("{}, error code: {}", message, code);
      throw std::runtime_error(format_str);
    });
    glfwInit();
    std::puts("GLFW initialized");
  }
  _Citrus_Sys_Context::~_Citrus_Sys_Context() {
    glfwTerminate();
  }
  _Citrus_Sys_Context _citrus_sys_ctx; // <-- We SHOULD reference this variable outside globals.cpp. This variable, when unused, makes the whole file to not be linked!!, this makes glfw not initialize
}