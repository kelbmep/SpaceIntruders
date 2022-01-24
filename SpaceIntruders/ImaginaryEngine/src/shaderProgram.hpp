#pragma once

#include <vector>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <functional>

class Texture;

class Uniform
{
public:
    std::function<void(void)> on_activate;
    virtual void activate()
    {
        if (on_activate)
        {
            on_activate();
        }
    };

    virtual ~Uniform() = default;
};

class TextureUniform : public Uniform
{
public:
    std::shared_ptr<Texture> texture;
    virtual void activate_with_slot(size_t) = 0;
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

class Vec3Uniform : public Uniform
{
public:
    glm::vec3 value;
};

class FloatUniform : public Uniform
{
public:
    float value;
};

class ShaderProgram : public std::enable_shared_from_this<ShaderProgram>
{
public:
    virtual ~ShaderProgram() = default;
    virtual void activate() = 0;

    virtual std::shared_ptr<TextureUniform> create_texture_uniform(std::string) = 0;
    virtual std::shared_ptr<Mat3Uniform> create_mat3_uniform(std::string) = 0;
    virtual std::shared_ptr<Vec2Uniform> create_vec2_uniform(std::string) = 0;
    virtual std::shared_ptr<Vec3Uniform> create_vec3_uniform(std::string) = 0;
    virtual std::shared_ptr<FloatUniform> create_float_uniform(std::string) = 0;

protected:
    std::vector<std::shared_ptr<Uniform>> _uniforms;
};