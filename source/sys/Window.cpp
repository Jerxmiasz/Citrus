#include <stdexcept>
#include "GLFW/glfw3.h"
#include "Citrus/sys/Window.hpp"

namespace citrus {
  Window::Window(std::string_view name, Vector2u size, bool isResizable, bool isDecorated, bool createOpenGlContext, bool isFullsceen, Monitor* monitor) {
    // TODO: Refactor everything i hate this
    if (createOpenGlContext) {
      
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    } else {
      glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NO_API);
    }
    glfwWindowHint(GLFW_DECORATED, isDecorated);
    glfwWindowHint(GLFW_RESIZABLE, isResizable);
    glfw_window_ = glfwCreateWindow(size.x, size.y, name.data(), (isFullsceen && monitor) ? monitor->getInternalMonitor() : nullptr, nullptr);
    if (!glfw_window_ && createOpenGlContext) {
      // There's still a chance, we can create a opengl context with a lower version
      glfwDefaultWindowHints();
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_DECORATED, isDecorated);
      glfwWindowHint(GLFW_RESIZABLE, isResizable);
      glfw_window_ = glfwCreateWindow(size.x, size.y, name.data(), (isFullsceen && monitor) ? monitor->getInternalMonitor() : nullptr, nullptr);
    }
   
    glfwSetWindowUserPointer(glfw_window_, static_cast<void*>(this));
    glfwSetKeyCallback(glfw_window_, [](GLFWwindow* window, int key, int scancode, int action, int mods){
      Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
      if (!win) {
        throw std::runtime_error("Lost window for recieving input");
      }
      switch (action) {
        case GLFW_RELEASE: {
          Event::KeyUp up_evt;
          up_evt.key = static_cast<Key>(key);
          up_evt.key_modifier = static_cast<KeyModifier>(mods);
          win->addEventToQueue(Event(up_evt));
          break;
        }
        case (GLFW_PRESS): {
          Event::KeyDown down_evt;
          down_evt.key = static_cast<Key>(key);
          down_evt.key_modifier = static_cast<KeyModifier>(mods);
          win->addEventToQueue(Event(down_evt));
          break;
        }
        case (GLFW_REPEAT): {
          Event::KeyRepeat keyrepeat_evt;
          keyrepeat_evt.key = static_cast<Key>(key);
          keyrepeat_evt.key_modifier = static_cast<KeyModifier>(mods);
          win->addEventToQueue(Event(keyrepeat_evt));
          break;
        }
      }
    });
    glfwSetWindowCloseCallback(glfw_window_, [](GLFWwindow* window) {
      Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
      win->addEventToQueue(Event(Event::Closed()));
    });
  }
  void Window::addEventToQueue(Event&& evt) {
    evt_queue_.push(std::forward<Event>(evt));
  }
  bool Window::isOpen() const {
    return !glfwWindowShouldClose(glfw_window_);
  }
  Vector2i Window::getSize() const {
    Vector2i size;
    glfwGetWindowSize(glfw_window_, &size.x, &size.y);
    return size;
  }

  std::optional<Window::Event> Window::getEvent() {
    glfwPollEvents();
    if (this->evt_queue_.empty()) {
      return std::nullopt;
    }
    Window::Event evt = this->evt_queue_.front();
    this->evt_queue_.pop();
    return evt;
  }

  void Window::setSize(Vector2i size) const {
    glfwSetWindowSize(glfw_window_, size.x, size.y);
  }

  void Window::close(){
    glfwSetWindowUserPointer(glfw_window_, nullptr);
    glfwDestroyWindow(glfw_window_);
  }

  Window::~Window() {
    if (glfw_window_) {
      glfwSetWindowUserPointer(glfw_window_, nullptr);
      glfwDestroyWindow(glfw_window_);
    }
  }

  void Window::setMonitor(Monitor& monitor) {
    citrus::VideoMode vidmode = monitor.getCurrentVideoMode();
    glfwSetWindowMonitor(glfw_window_, monitor.getInternalMonitor(), 0, 0, vidmode.size.x, vidmode.size.y, vidmode.refresh_rate);
    monitor_ = std::addressof(monitor);
  }
  Monitor* Window::getMonitor() const {
    return monitor_;
  }

  GLFWwindow* Window::getGlfwPtr() const noexcept {
    return glfw_window_;
  }
}