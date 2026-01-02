#ifndef CITRUS_GRAPHICS_COLOR
#define CITRUS_GRAPHICS_COLOR

#include <cstdint>
#include <array>

namespace citrus {
  class Color {
    public:
    Color() = delete;
    Color(float r, float g, float b, float a) : r_(r), g_(g), b_(b), a_(a) {}
    static Color FromByteRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
      return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }
    static Color FromFloatRGBA(float r, float g, float b, float a = 1.f) {
      return Color(r, g, b, a);
    }
    ~Color() = default;
    std::array<float, 4> asFloatRgba() noexcept {
      return std::array<float, 4>{r_, g_, b_, a_};
    }
    std::array<uint8_t, 4> asUint8Rgba() const noexcept {
      return {static_cast<uint8_t>(r_ * 255), static_cast<uint8_t>(g_ * 255), static_cast<uint8_t>(b_ * 255), static_cast<uint8_t>(a_ * 255)};
    }
    private:
    float r_,g_,b_,a_;
  };
}

#endif