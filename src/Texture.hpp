#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <epoxy/gl.h>
#include <string>

namespace Diggler {
class Texture {
public:
  enum class PixelFormat {
    RGB,
    RGBA,
    Monochrome8
  };
  enum class Filter {
    Nearest,
    Linear
  };
  enum class Wrapping {
    Repeat,
    Clamp
  };
private:
  GLuint m_id;
  uint m_w, m_h;
  PixelFormat m_format;
  void create();
  void setPlaceholder();

public:
  Texture(uint w, uint m_h, PixelFormat format = PixelFormat::RGB);
  Texture(uint w, uint m_h, const uint8_t *data, PixelFormat format = PixelFormat::RGB);
  Texture(const std::string &path, PixelFormat format = PixelFormat::RGB);
  ~Texture();

  inline GLuint id() const { return m_id; }
  inline operator GLuint() const { return m_id; }

  void resize(uint w, uint h);

  uint w() const {
    return m_w;
  }

  uint h() const {
    return m_h;
  }


  PixelFormat pixelFormat();
  uint requiredBufferSize();
  void getTexture(uint8_t *data);
  void setTexture(uint w, uint h, const uint8_t *data, PixelFormat format = PixelFormat::RGB);
  void setTexture(const uint8_t *data, PixelFormat format = PixelFormat::RGB);
  void setSubTexture(int x, int y, uint w, uint h, const uint8_t *data, PixelFormat format = PixelFormat::RGB);
  void setFiltering(Filter min, Filter mag);
  void setWrapping(Wrapping s, Wrapping t);


  void bind() const {
    glBindTexture(GL_TEXTURE_2D, m_id);
  }

  void bind(uint number) const;

  static void unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
  }
};
}

#endif
