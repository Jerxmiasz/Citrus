#ifndef CITRUS_SYS_WINDOW_HPP
#define CITRUS_SYS_WINDOW_HPP

#include <string_view>
#include <variant>
#include "Keyboard.hpp"
#include "Vector2.hpp"
#include "Monitor.hpp"

struct GLFWwindow;

namespace citrus {
  class Window {
    public:

    class Event {
      public:
      class Closed {};
      class KeyDown {
        citrus::Key key;
        citrus::KeyModifier key_modifier;
      };
      class KeyUp {
        citrus::Key key;
        citrus::KeyModifier key_modifier;
      };
      template<typename TEvt>
      bool matches() {
        return std::holds_alternative<TEvt>(internal_evt_);
      }
      private:
      std::variant<Closed, KeyDown, KeyUp> internal_evt_;
    };
    
    Window() = delete;

    explicit Window(std::string_view name, Vector2u size, bool isFullsceen = false, bool isResizable = true, bool isDecorated = true ,Monitor* monitor = nullptr);
    Vector2u getSize() const;

    void setSize() const;

    void* getGlfwPtr() noexcept;
    // Gets the monitor assigned to the window
    Monitor getMonitor() const;

    void setMonitor(Monitor& monitor);

    void close();

    void isOpen();

    ~Window();

    private:
    Monitor* monitor_;
    GLFWwindow* glfw_window_;
  };
}

#endif