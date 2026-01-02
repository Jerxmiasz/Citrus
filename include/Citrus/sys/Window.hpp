#ifndef CITRUS_SYS_WINDOW_HPP
#define CITRUS_SYS_WINDOW_HPP

#include <string_view>
#include <variant>
#include <optional>
#include <queue>
#include "Keyboard.hpp"
#include "Vector2.hpp"
#include "Monitor.hpp"

struct GLFWwindow;

namespace citrus {
  class Window {
    public:

    

    class Event {
      public:
      struct Closed {};
      struct KeyRepeat {
        citrus::Key key;
        citrus::KeyModifier key_modifier;
      };
      struct KeyDown {
        citrus::Key key;
        citrus::KeyModifier key_modifier;
      };
      struct KeyUp {
        citrus::Key key;
        citrus::KeyModifier key_modifier;
      };
      using EventsVariant = std::variant<Closed, KeyDown, KeyUp, KeyRepeat>;
      Event() = delete;
      Event(const Event& cpy) : internal_evt_(cpy.internal_evt_) {}
      Event(EventsVariant&& evt) : internal_evt_(std::move(evt)) {}
      template<typename TEvt>
      bool matches() {
        return std::holds_alternative<TEvt>(internal_evt_);
      }
      template <typename TEvt>
      TEvt get() {
        return std::get<TEvt>(internal_evt_);
      }
      private:
      EventsVariant internal_evt_;
    };
    
    Window() = delete;

    explicit Window(std::string_view name, Vector2u size, bool isResizable = true, bool isDecorated = true , bool createOpenGlContext = true, bool isFullsceen = false,Monitor* monitor = nullptr);
    
    Vector2i getSize() const;

    std::optional<Event> getEvent();  
    
    void addEventToQueue(Event&& evt);

    void setSize(Vector2i size) const;

    GLFWwindow* getGlfwPtr() const noexcept;
    // Gets the monitor assigned to the window
    Monitor* getMonitor() const;

    void setMonitor(Monitor& monitor);

    void close();

    bool isOpen() const ;

    ~Window();

    private:
    std::queue<Event> evt_queue_;
    Monitor* monitor_;
    GLFWwindow* glfw_window_;
  };
}

#endif