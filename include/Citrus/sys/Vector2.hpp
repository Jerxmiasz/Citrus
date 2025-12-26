#ifndef CITRUS_SYS_VECTOR2_HPP
#define CITRUS_SYS_VECTOR2_HPP

namespace citrus {
  template <typename T>
  struct Vector2 {
    T x, y;
  };
  using Vector2i = Vector2<int>;
  using Vector2f = Vector2<float>;
  using Vector2u = Vector2<float>;
}

#endif