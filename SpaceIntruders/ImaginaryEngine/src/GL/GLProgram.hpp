#pragma once

#include <shaderProgram.hpp>
#include <cstdint>
#include <string>
#include <glm/glm.hpp>

class GLProgram : public ShaderProgram
{
public:
    GLProgram(std::string, std::string);
    ~GLProgram() override;

    std::shared_ptr<TextureUniform> createTextureUniform(std::string) override;
    std::shared_ptr<Mat3Uniform> createMat3Uniform(std::string) override;
    std::shared_ptr<Vec2Uniform> createVec2Uniform(std::string) override;

    void activate() override;
    uint32_t getProgramId() const { return _program; }
protected:
    uint32_t _vertexShader;
    uint32_t _fragmentShader;
    uint32_t _program;
};

class GLTextureUniform : public TextureUniform
{
public:
    explicit GLTextureUniform(const std::shared_ptr<GLProgram>&, std::string);
    void activate() override;
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