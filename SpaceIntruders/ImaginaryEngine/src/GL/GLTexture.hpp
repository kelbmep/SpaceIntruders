#pragma once

#include <texture.hpp>

class Bitmap;

class GLTexture : public Texture
{
public:
    explicit GLTexture(Bitmap, bool);
    void active();

    uint32_t get_ID() const;
private:
    uint32_t _ID;
};
