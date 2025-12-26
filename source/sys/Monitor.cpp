#include <algorithm>
#include "Citrus/sys/Globals.hpp"
#include <stdexcept>
#include "GLFW/glfw3.h"
#include "Citrus/sys/Monitor.hpp"

static std::function<void(citrus::Monitor&, citrus::Monitor::EventType)> s_citrus_monitor_evt_callback = nullptr;

static void glfw_evt_callback(GLFWmonitor* monitor, int event) {
  if (event == GLFW_DISCONNECTED)
  {
    citrus::Monitor *_monitor = static_cast<citrus::Monitor *>(glfwGetMonitorUserPointer(monitor));
    _monitor->setConnected(false);
    if (s_citrus_monitor_evt_callback != nullptr) {
      s_citrus_monitor_evt_callback(*_monitor, citrus::Monitor::EventType::DISCONNECTED);
    }
    delete _monitor;
    glfwSetMonitorUserPointer(monitor, nullptr); // Now the glfwmonitor doesnt have any citrus::Monitor, the glfw monitor will be null at some point, luckily,
  }
  if (event == GLFW_CONNECTED)
  {
    citrus::Monitor *_monitor = new citrus::Monitor(monitor);
    _monitor->setConnected(true);
    glfwSetMonitorUserPointer(monitor, _monitor);
    if (s_citrus_monitor_evt_callback != nullptr) 
      s_citrus_monitor_evt_callback(*_monitor, citrus::Monitor::EventType::CONNECTED);
  };
}

static citrus::VideoMode _ToCitrusVideoMode(GLFWvidmode vidmode) {
  citrus::VideoMode citrusvidmode;
  citrusvidmode.blue_bits = vidmode.blueBits;
  citrusvidmode.green_bits = vidmode.greenBits;
  citrusvidmode.red_bits = vidmode.redBits;
  citrusvidmode.refresh_rate = vidmode.refreshRate;
  citrusvidmode.size.x = vidmode.width;
  citrusvidmode.size.y = vidmode.height;
  return citrusvidmode;
}


namespace citrus {
  
  extern _Citrus_Sys_Context _citrus_sys_ctx;

  Monitor::Monitor(GLFWmonitor* monitor) {
    if (!_citrus_sys_ctx.monitor_callabck_set) {
      glfwSetMonitorCallback(glfw_evt_callback);
     _citrus_sys_ctx.monitor_callabck_set = true;
    }
    this->setInternalMonitor(monitor);
  }

  Vector2i Monitor::getPos() const {
    int x,y;
    glfwGetMonitorPos(this->internal_monitor_, &x, &y);
    return Vector2i(x,y);
  }

  Vector2i Monitor::getWorkAreaPosition() const {
    Vector2i pos;
    glfwGetMonitorWorkarea(this->internal_monitor_, &pos.x, &pos.y, nullptr, nullptr);
    return pos;
  }

  Vector2i Monitor::getWorkAreaSize() const {
    Vector2i size;
    glfwGetMonitorWorkarea(this->internal_monitor_, nullptr, nullptr, &size.x, &size.y);
    return size;
  }

  Vector2i Monitor::getPhysicalSize() const {
    Vector2i psize;
    glfwGetMonitorPhysicalSize(this->internal_monitor_, std::addressof(psize.x), std::addressof(psize.y));
    return psize;
  }

  Vector2f Monitor::getContentScale() const {
    Vector2f content_scale;
    glfwGetMonitorContentScale(this->internal_monitor_, std::addressof(content_scale.x), std::addressof(content_scale.y));
    return content_scale;
  }

  std::string_view Monitor::getName() const {
    return glfwGetMonitorName(this->internal_monitor_);
  }

  std::vector<VideoMode> Monitor::getSupportedVideomodes() const {
    int size;
    const GLFWvidmode* vidmodes = glfwGetVideoModes(this->internal_monitor_, std::addressof(size));
    std::vector<VideoMode> vidmode_vec(size);
    std::transform(vidmodes, vidmodes + size, vidmode_vec.begin(), [](const GLFWvidmode& vidmode){
      return _ToCitrusVideoMode(vidmode);
    });
    return vidmode_vec;
  }

  VideoMode Monitor::getCurrentVideoMode() const {
    return _ToCitrusVideoMode(*glfwGetVideoMode(this->internal_monitor_));
  }

  void Monitor::setInternalMonitor(GLFWmonitor* monitor) {
    this->internal_monitor_ = monitor;
  }

  Monitor GetMainMonitor() {
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    if (!primary) {
      throw std::runtime_error("No monitors attached");
    }
    citrus::Monitor* userptr = static_cast<citrus::Monitor*>(glfwGetMonitorUserPointer(primary));
    if (userptr) return *userptr;
    return Monitor(primary);
  }
  std::vector<Monitor> GetConnectedMonitors() {
    int size;
    GLFWmonitor** monitors_arr = glfwGetMonitors(std::addressof(size));
    auto monitors_vec = std::vector<Monitor>();
    std::transform(monitors_arr, monitors_arr + size, monitors_vec.begin(), [](GLFWmonitor* monitor){
      return Monitor(*static_cast<Monitor*>(glfwGetMonitorUserPointer(monitor)));
    });
    return monitors_vec;
  }
  bool Monitor::isConnected() const {
    return connected_;
  }
  void SetMonitorCallback(std::function<void(Monitor, Monitor::EventType)> _Callback) {
    s_citrus_monitor_evt_callback = _Callback;
  }

}