#include <GL/GLParticleBuffer.hpp>
#include <GL/glew.h>

GLParticleBuffer::GLParticleBuffer(std::vector<ParticleData> data)
{
    glGenVertexArrays(1, &_VAO);

    glBindVertexArray(_VAO);

    glGenBuffers(1, &_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(ParticleData), data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2,
        GL_FLOAT, GL_FALSE, sizeof(ParticleData), (void*)offsetof(ParticleData, pos));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2,
        GL_FLOAT, GL_FALSE, sizeof(ParticleData), (void*)offsetof(ParticleData, velocity));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4,
        GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ParticleData), (void*)offsetof(ParticleData, color));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    _count = data.size();
}

void GLParticleBuffer::draw()
{
    glBindVertexArray(_VAO);
    glDrawArrays(GL_POINTS, 0, _count);
}
