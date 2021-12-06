#pragma once

#include <bitmap.hpp>
#include <texture.hpp>

class GLTexture : public Texture
{
public:
    explicit GLTexture(Bitmap);
    void active();
private:
    uint32_t _id;
};
