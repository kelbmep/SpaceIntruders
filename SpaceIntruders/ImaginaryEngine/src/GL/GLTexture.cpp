#include <GL/GLTexture.hpp>
#include <GL/glew.h>
#include <bitmap.hpp>

GLTexture::GLTexture(Bitmap bitmap)
    : Texture(bitmap.get_size())
{
    glGenTextures(1, &_ID);
    glBindTexture(GL_TEXTURE_2D, _ID);

    auto ch = bitmap.get_color_channels();
    
    const auto internal_format = [&]()
    {
        switch (ch)
        {
        case 1: return GL_R8;
        case 2: return GL_RG;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default: return NULL;
        }
    }();

    const auto format = [&]()
    {
        switch (ch)
        {
        case 1: return GL_RED;
        case 2: return GL_RG;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default: return NULL;
        }
    }();

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, (GLsizei)bitmap.get_size().x, (GLsizei)bitmap.get_size().y, 0,
                 format, GL_UNSIGNED_BYTE, bitmap.get_image().data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);
}

void GLTexture::active()
{
    glBindTexture(GL_TEXTURE_2D, _ID);
}