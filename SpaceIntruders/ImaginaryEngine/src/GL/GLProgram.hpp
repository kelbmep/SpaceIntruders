#pragma once

#include <shaderProgram.hpp>
#include <cstdint>
#include <string>
#include <glm/glm.hpp>

class Engine;

class GLProgram : public ShaderProgram
{
public:
    GLProgram(std::initializer_list<const char*>, std::string, std::string);
    //GLProgram(std::string, std::string);
    ~GLProgram() override;

    std::string read_from_file(std::string);

    std::shared_ptr<TextureUniform> create_texture_uniform(std::string) override;
    std::shared_ptr<Mat3Uniform> create_mat3_uniform(std::string) override;
    std::shared_ptr<Vec2Uniform> create_vec2_uniform(std::string) override;
    std::shared_ptr<Vec3Uniform> create_vec3_uniform(std::string) override;
    std::shared_ptr<FloatUniform> create_float_uniform(std::string) override;

    void activate() override;
    uint32_t get_program_ID() const { return _program; }
protected:
    uint32_t _vertex_shader;
    uint32_t _fragment_shader;
    uint32_t _program;
};

class GLTextureUniform : public TextureUniform
{
public:
    explicit GLTextureUniform(const std::shared_ptr<GLProgram>&, std::string);
    void activate() override;
    void activate_with_slot(size_t) override;
private:
    int32_t _location;
};

class GLMat3Uniform : public Mat3Uniform
{
public:
    explicit GLMat3Uniform(const std::shared_ptr<GLProgram>&, std::string);
    void activate() override;
private:
    int32_t _location;
};

class GLVec2Uniform : public Vec2Uniform
{
public:
    explicit GLVec2Uniform(const std::shared_ptr<GLProgram>&, std::string);
    void activate() override;
private:
    int32_t _location;
};

class GLVec3Uniform : public Vec3Uniform
{
public:
    explicit GLVec3Uniform(const std::shared_ptr<GLProgram>&, std::string);
    void activate() override;
private:
    int32_t _location;
};

class GLFloatUniform : public FloatUniform
{
public:
    explicit GLFloatUniform(const std::shared_ptr<GLProgram>&, std::string);
    void activate() override;
private:
    int32_t _location;
};