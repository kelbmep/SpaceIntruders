#include <GL/GLVertexBuffer.hpp>
#include <GL/GLHeaders.hpp>
#include "GLHeaders.hpp"

GLVertexBuffer::GLVertexBuffer(const Engine& engine, MeshData data)
    : _engine(engine)
{
#if GLES20
    glGenVertexArraysOES(1, &_VAO);

    glBindVertexArrayOES(_VAO);
#elif GL33
    glGenVertexArrays(1, &_VAO);

    glBindVertexArray(_VAO);
#endif
    glGenBuffers(1, &_VBO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(MeshData::Vertex), data.vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2,
                          GL_FLOAT, GL_FALSE, sizeof(MeshData::Vertex), (void*)offsetof(MeshData::Vertex, pos));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2,
                          GL_FLOAT, GL_FALSE, sizeof(MeshData::Vertex), (void*)offsetof(MeshData::Vertex, texcoord));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4,
                          GL_UNSIGNED_BYTE, GL_TRUE, sizeof(MeshData::Vertex), (void*)offsetof(MeshData::Vertex, color));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 data.indexes.size() * sizeof(std::uint32_t),
                 data.indexes.data(), GL_STATIC_DRAW);

    _count = static_cast<std::uint32_t>(data.indexes.size());
}

GLVertexBuffer::GLVertexBuffer(const Engine& engine, const VertexData&)
    : _engine(engine)
{

}

void GLVertexBuffer::draw()
{
    this->draw(_count, 0);
}

void GLVertexBuffer::draw(size_t count, size_t pos)
{
#if GLES20
    glBindVertexArrayOES(_VAO);
#elif GL33
    glBindVertexArray(_VAO);
#endif

    if (_IBO != -1)
    {
        glDrawElements(GL_TRIANGLES, (GLsizei)count, GL_UNSIGNED_INT, reinterpret_cast<void*>(pos));
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, pos, count);
    }
}

GLVertexBuffer::~GLVertexBuffer()
{
    glDeleteBuffers(1, &_VBO);
    if (_IBO != -1)
    {
        glDeleteBuffers(1, &_IBO);
    }
#if GLES20
    glDeleteVertexArraysOES(1, &_VAO);
#elif GL33
    glDeleteVertexArrays(1, &_VAO);
#endif
}