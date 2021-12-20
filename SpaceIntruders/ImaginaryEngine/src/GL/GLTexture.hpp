#pragma once

#include <texture.hpp>

class Bitmap;

class GLTexture : public Texture
{
public:
    explicit GLTexture(Bitmap);
    void active();
private:
    uint32_t _ID;
};
