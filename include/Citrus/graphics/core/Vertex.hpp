#ifndef CITRUS_GRAPHICS_VERTEX_HPP
#define CITRUS_GRAPHICS_VERTEX_HPP

#include <span>

#include "color.hpp"
#include "Citrus/sys/Vector2.hpp"
#include "Citrus/sys/Vector3.hpp"


namespace citrus {
  struct Vertex {
    Vertex() : color(1.f, 1.f, 1.f, 1.f), position() {}
    Vertex(Color _color, Vector3f pos) : color(_color), position(pos) {};
    Color color;
    Vector3f position;
  };

  class VertexBuffer {
    public:
    static inline constexpr uint8_t SMALL_BUFFER_MAX_SIZE = 16;
    
    VertexBuffer() = default;
    VertexBuffer(std::initializer_list<Vertex> vertex_container) {
      if (vertex_container.size() > SMALL_BUFFER_MAX_SIZE) {
        vertices_ = new Vertex[vertex_container.size()];
        std::copy(vertex_container.begin(), vertex_container.end(), vertices_);
      } else {
        std::copy(vertex_container.begin(), vertex_container.end(), sbo_buffer_);
        vertices_ = sbo_buffer_;
      }
      size_ = vertex_container.size();
    }
    VertexBuffer(std::span<Vertex> vertex_container) {
      if (vertex_container.size() > SMALL_BUFFER_MAX_SIZE) {
        vertices_ = new Vertex[vertex_container.size()];
        std::copy(vertex_container.begin(), vertex_container.end(), vertices_);
      } else {
        std::copy(vertex_container.begin(), vertex_container.end(), sbo_buffer_);
        vertices_ = sbo_buffer_;
      }
      size_ = vertex_container.size();
    }
    VertexBuffer(Vertex* vertices, size_t size) {
      if (size > SMALL_BUFFER_MAX_SIZE) {
        vertices_ = new Vertex[size];
        std::copy(vertices, vertices + size, vertices_);
      } else {
        std::copy(vertices, vertices + size, sbo_buffer_);
        vertices_ = sbo_buffer_;
      }
      size_ = size;
    }
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;
    VertexBuffer(VertexBuffer&& other) {
      if (other.size_ < SMALL_BUFFER_MAX_SIZE) {
        std::copy(other.sbo_buffer_, other.sbo_buffer_ + other.size_, sbo_buffer_);
      }
      vertices_ = other.vertices_;
      size_ = other.size_;
    }
    VertexBuffer& operator=(VertexBuffer&&) = default;
    ~VertexBuffer() {
      if (size_ > SMALL_BUFFER_MAX_SIZE) {
        delete[] vertices_;
      }
    }

    std::span<Vertex> getVertices() const {
      return std::span<Vertex>(vertices_, size_);
    }
    bool isEmpty() const noexcept {
      if (size_ == 0) {
        return true;
      }
      return false;
    }

    private:
    Vertex* vertices_ = nullptr;
    Vertex sbo_buffer_[SMALL_BUFFER_MAX_SIZE];
    size_t size_ = 0;
  };

}

#endif