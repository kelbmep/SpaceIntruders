#include <GL/GLFrameBuffer.hpp>
#include <GL/GLTexture.hpp>
#include <render.hpp>
#include <GL/GLHeaders.hpp>

GLFrameBuffer::GLFrameBuffer(const Engine& engine, glm::vec2 size)
{
    GLint curFBO;

#if GLES20
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING_NV, &curFBO);
#elif GL33
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &curFBO);
#endif

    glGenFramebuffers(1, &_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    _texture = engine.get_render().create_texture(Bitmap{ 4, std::vector<unsigned char>(NULL), size });
    auto glTexture = std::static_pointer_cast<GLTexture>(_texture);

    glTexture->active();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glTexture->get_ID(), 0);

    glBindFramebuffer(GL_FRAMEBUFFER, curFBO);
}

GLFrameBuffer::~GLFrameBuffer()
{
    glDeleteFramebuffers(1, &_fbo);
}

void GLFrameBuffer::draw_to_frame_buffer(std::function<void(void)> callback)
{
    GLint curFBO;

#if GLES20
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING_NV, &curFBO);
#elif GL33
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &curFBO);
#endif

    GLint curViewport[4];
    glGetIntegerv(GL_VIEWPORT, curViewport);

    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

    glDisable(GL_SCISSOR_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, _texture->get_size().x, _texture->get_size().y);
    glEnable(GL_SCISSOR_TEST);

    callback();

    glBindFramebuffer(GL_FRAMEBUFFER, curFBO);
    glViewport(curViewport[0], curViewport[1], curViewport[2], curViewport[3]);
}