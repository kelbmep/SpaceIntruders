#include <GL/GLTexture.hpp>
#include <GL/glew.h>

//#include "GlHeaders.hpp"

/*namespace
{
    GLenum getGLInternalFormat(Bitmap::Format bitmapFormat)
    {
        if (bitmapFormat == Bitmap::Format::R)
        {
            return GL_R8;
        }

        if (bitmapFormat == Bitmap::Format::Rgba32F || bitmapFormat == Bitmap::Format::Rgb32F)
        {
            return GL_RGBA32F;
        }
        else if (bitmapFormat == Bitmap::Format::Rgba16F)
        {
            return GL_RGBA16F;
        }
        else if (bitmapFormat == Bitmap::Format::Rgb16F)
        {
            return GL_RGB16F;
        }
        else if (bitmapFormat == Bitmap::Format::Rgb)
        {
            return GL_RGB;
        }
        else
        {
            return GL_RGBA;
        }
    }

    GLenum getGlFormat(Bitmap::Format bitmapFormat)
    {
        switch (bitmapFormat)
        {
        case Bitmap::Format::R:
            return GL_RED;

        case Bitmap::Format::Rgb:
        case Bitmap::Format::Rgb16F:
        case Bitmap::Format::Rgb32F:
            return GL_RGB;

        case Bitmap::Format::Rgba:
        case Bitmap::Format::Rgba16F:
        case Bitmap::Format::Rgba32F:
            return GL_RGBA;

        }
    }

    GLenum getGlType(Bitmap::Format bitmapFormat)
    {
        switch (bitmapFormat)
        {
        case Bitmap::Format::Rgb32F:
        case Bitmap::Format::Rgba32F:
            return GL_FLOAT;

        case Bitmap::Format::Rgb16F:
        case Bitmap::Format::Rgba16F:
            return GL_HALF_FLOAT;

        case Bitmap::Format::Rgb:
        case Bitmap::Format::Rgba:
        case Bitmap::Format::R:
            return GL_UNSIGNED_BYTE;

        }
    }
}
*/
GLTexture::GLTexture(Bitmap bitmap)
    : Texture(bitmap.get_size())
{
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);

    int ch = bitmap.get_color_channels();
    
    const auto internal_format = [&]()
    {
        switch (ch)
        {
        case 1: return GL_R8;
        case 2: return GL_RG;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
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
        }
    }();

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, bitmap.get_size().x, bitmap.get_size().y, 0,
                format, GL_UNSIGNED_BYTE, bitmap.get_image().data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);
}

void GLTexture::active()
{
    glBindTexture(GL_TEXTURE_2D, _id);
}