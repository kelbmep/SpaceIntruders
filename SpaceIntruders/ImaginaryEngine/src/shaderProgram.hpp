#pragma once

#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>

class Texture;

class Uniform
{
public:
    virtual void activate() = 0;
    virtual ~Uniform() = default;
};

class TextureUniform : public Uniform
{
public:
    std::shared_ptr<Texture> texture;
};

class Mat3Uniform : public Uniform
{
public:
    glm::mat3 value;
};

class Vec2Uniform : public Uniform
{
public:
    glm::vec2 value;
};

class ShaderProgram : public std::enable_shared_from_this<ShaderProgram>
{
public:
    virtual ~ShaderProgram() = default;
    virtual void activate();

    virtual std::shared_ptr<TextureUniform> create_texture_uniform(std::string) = 0;
    virtual std::shared_ptr<Mat3Uniform> create_mat3_uniform(std::string) = 0;
    virtual std::shared_ptr<Vec2Uniform> create_vec2_uniform(std::string) = 0;

protected:
    std::vector<std::shared_ptr<Uniform>> _uniforms;
};

inline void ShaderProgram::activate()
{
    for (const auto& uniform : _uniforms)
    {
        uniform->activate();
    }
}