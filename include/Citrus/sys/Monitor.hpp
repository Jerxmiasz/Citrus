#ifndef CITRUS_SYS_MONITOR
#define CITRUS_SYS_MONITOR

#include "Globals.hpp"
#include <vector>
#include <string_view>
#include <functional>
#include "VideoMode.hpp"

struct GLFWmonitor;

namespace citrus {
  class Monitor {
    public:

    enum EventType {
      CONNECTED,
      DISCONNECTED
    };

    Monitor() = delete;
    explicit Monitor(GLFWmonitor* monitor);

    Vector2i getPos() const;

    Vector2i getWorkAreaPosition() const;

    Vector2i getWorkAreaSize() const;
    // Gets the width and height of the monitor in millimeters
    Vector2i getPhysicalSize() const;

    Vector2f getContentScale() const;

    std::string_view getName() const;

    std::vector<VideoMode> getSupportedVideomodes() const;

    VideoMode getCurrentVideoMode() const;

    void setInternalMonitor(GLFWmonitor* monitor);
    GLFWmonitor* getInternalMonitor();

    bool isConnected() const;

    void setConnected(bool isconnected) noexcept {connected_ = isconnected;};

    private:
    bool connected_;
    GLFWmonitor* internal_monitor_;
  };
  Monitor GetMainMonitor();
  std::vector<Monitor> GetConnectedMonitors();
  void SetMonitorCallback(std::function<void(Monitor, Monitor::EventType)> monitor);

}

#endif