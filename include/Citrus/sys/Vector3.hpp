#ifndef CITRUS_SYS_VECTOR3_HPP
#define CITRUS_SYS_VECTOR3_HPP

namespace citrus {
  template <typename T>
  struct Vector3 {
    T x, y, z;
  };
  using Vector3i = Vector3<int>;
  using Vector3f = Vector3<float>;
  using Vector3u = Vector3<float>;
}

#endif