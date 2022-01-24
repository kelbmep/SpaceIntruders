#include <GL/GLParticleBuffer.hpp>
#include <GL/GLHeaders.hpp>
#include <stdexcept>

GLParticleBuffer::GLParticleBuffer(std::vector<ParticleData> data)
{
    this->create_buffers();

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(ParticleData), data.data(), GL_STATIC_DRAW);

    this->create_layouts();

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    _count = data.size();
}

GLParticleBuffer::GLParticleBuffer(size_t capacity)
{
    this->create_buffers();

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, capacity * sizeof(ParticleData), nullptr, GL_STREAM_DRAW);

    this->create_layouts();

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    _count = 0;
    _cur_pos = 0;
    _capacity = capacity;

    _is_dynamic = true;
}

void GLParticleBuffer::draw()
{
    if (_count > 0)
    {
#if GLES20
        glBindVertexArrayOES(_VAO);
#elif GL33
        glBindVertexArray(_VAO);
#endif

        glDrawArrays(GL_POINTS, 0, _count);
    }
}

void GLParticleBuffer::create_buffers()
{
#if GLES20
    glGenVertexArraysOES(1, &_VAO);

    glBindVertexArrayOES(_VAO);
#elif GL33
    glGenVertexArrays(1, &_VAO);

    glBindVertexArray(_VAO);
#endif

    glGenBuffers(1, &_VBO);
}

void GLParticleBuffer::create_layouts()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2,
        GL_FLOAT, GL_FALSE, sizeof(ParticleData), (void*)offsetof(ParticleData, pos));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2,
        GL_FLOAT, GL_FALSE, sizeof(ParticleData), (void*)offsetof(ParticleData, velocity));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4,
        GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ParticleData), (void*)offsetof(ParticleData, color));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1,
        GL_FLOAT, GL_FALSE, sizeof(ParticleData), (void*)offsetof(ParticleData, phase));
}

void GLParticleBuffer::add_data(ParticleBuffer::ParticleData data)
{
    if (!_is_dynamic)
    {
        throw std::logic_error("Cannot update static buffer");
    }

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferSubData(GL_ARRAY_BUFFER, _cur_pos * sizeof(ParticleData), sizeof(ParticleData), &data);

    ++_cur_pos;

    if (_count < _capacity)
    {
        ++_count;
    }

    _cur_pos %= _capacity;
}

GLParticleBuffer::~GLParticleBuffer()
{
    glDeleteBuffers(1, &_VBO);

#if GLES20
    glDeleteVertexArraysOES(1, &_VAO);
#elif GL33
    glDeleteVertexArrays(1, &_VAO);
#endif
}
