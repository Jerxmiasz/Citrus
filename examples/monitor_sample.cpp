#include "Citrus/sys/Monitor.hpp"
#include <iostream>

int main() {
  citrus::Monitor monitor = citrus::GetMainMonitor();
  std::cout << monitor.getName() << '\n';
  std::cout << monitor.getCurrentVideoMode().refresh_rate << '\n';
  std::cout << monitor.isConnected() << '\n';
  std::cout << monitor.getPhysicalSize().x << ", " <<  monitor.getPhysicalSize().y;
}