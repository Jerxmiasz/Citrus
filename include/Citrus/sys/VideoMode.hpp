#ifndef CITRUS_SYS_VIDEOMODE_HPP
#define CITRUS_SYS_VIDEOMODE_HPP

#include "Vector2.hpp"

namespace citrus {
  struct VideoMode {
    Vector2i size;
    int red_bits;
    int green_bits;
    int blue_bits;
    int refresh_rate;
  };
}

#endif