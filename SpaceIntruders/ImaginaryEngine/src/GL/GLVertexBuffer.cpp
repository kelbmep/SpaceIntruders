#include <GL/GLVertexBuffer.hpp>
#include <GL/glew.h>
#include <render.hpp>

GLVertexBuffer::GLVertexBuffer(const Engine& engine,
    MeshData data)
    : _engine(engine)
{
    glGenVertexArrays(1, &_VAO);

    glBindVertexArray(_VAO);

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

void GLVertexBuffer::draw()
{
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, _count, GL_UNSIGNED_INT, 0);
}

void GLVertexBuffer::draw(size_t count, size_t pos)
{
    glBindVertexArray(_VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)count, GL_UNSIGNED_INT, reinterpret_cast<void*>(pos));
}

GLVertexBuffer::~GLVertexBuffer()
{
    glDeleteBuffers(1, &_VBO);
    glDeleteBuffers(1, &_IBO);
    glDeleteVertexArrays(1, &_VAO);
}