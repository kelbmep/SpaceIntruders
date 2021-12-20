#include <GL/GLProgram.hpp>
#include <GL/GLTexture.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <iostream>
#include <string>

GLProgram::GLProgram(std::string vs_s, std::string ps_s)
{
    const auto* vs = vs_s.c_str();
    const auto* ps = ps_s.c_str();
    _vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(_vertex_shader, 1, &vs, nullptr);
    glCompileShader(_vertex_shader);

    GLint success;
    GLchar info_log[512];
    glGetShaderiv(_vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(_vertex_shader, 512, nullptr, info_log);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    _fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_fragment_shader, 1, &ps, nullptr);
    glCompileShader(_fragment_shader);

    glGetShaderiv(_fragment_shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(_fragment_shader, 512, nullptr, info_log);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    _program = glCreateProgram();

    glAttachShader(_program, _vertex_shader);
    glAttachShader(_program, _fragment_shader);
    glLinkProgram(_program);

    glGetProgramiv(_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(_program, 512, nullptr, info_log);
        std::cerr << "ERROR::SHADER::LINKING_FAILED\n" << info_log << std::endl;
    }

    GLint i;
    GLint count;

    GLint size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei buf_size = 16; // maximum name length
    GLchar name[buf_size]; // variable name in GLSL
    GLsizei length; // name length

    glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTES, &count);
    printf("Active Attributes: %d\n", count);

    for (i = 0; i < _program; i++)
    {
        glGetActiveAttrib(_program, (GLuint)i, buf_size, &length, &size, &type, name);

        printf("Attribute #%d Type: %u Name: %s\n", i, type, name);
    }

    glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &count);
    printf("Active Uniforms: %d\n", count);

    for (i = 0; i < count; i++)
    {
        glGetActiveUniform(_program, (GLuint)i, buf_size, &length, &size, &type, name);

        printf("Uniform #%d Type: %u Name: %s\n", i, type, name);
    }
}

GLProgram::~GLProgram()
{
    glDeleteProgram(_program);
    glDeleteShader(_vertex_shader);
    glDeleteShader(_fragment_shader);
}

void GLProgram::activate()
{
    glUseProgram(_program);
    ShaderProgram::activate();
}

std::shared_ptr<TextureUniform> GLProgram::create_texture_uniform(std::string name)
{
    auto uniform = std::make_shared<GLTextureUniform>(std::static_pointer_cast<GLProgram>(shared_from_this()), name);
    _uniforms.push_back(uniform);
    return uniform;
}

std::shared_ptr<Mat3Uniform> GLProgram::create_mat3_uniform(std::string name)
{
    auto uniform = std::make_shared<GLMat3Uniform>(std::static_pointer_cast<GLProgram>(shared_from_this()), name);
    _uniforms.push_back(uniform);
    return uniform;
}

std::shared_ptr<Vec2Uniform> GLProgram::create_vec2_uniform(std::string name)
{
    auto uniform = std::make_shared<GLVec2Uniform>(std::static_pointer_cast<GLProgram>(shared_from_this()), name);
    _uniforms.push_back(uniform);
    return uniform;
}

void GLTextureUniform::activate()
{
    auto glTexture = std::dynamic_pointer_cast<GLTexture>(texture);
    if (glTexture)
    {
        //TODO: diff texture slots
        glActiveTexture(GL_TEXTURE0);
        glTexture->active();

        glUniform1i(_location, 0);
    }
}

GLTextureUniform::GLTextureUniform(const std::shared_ptr<GLProgram>& program, std::string name)
{
    _location = glGetUniformLocation(program->get_program_ID(), name.data());
}

GLMat3Uniform::GLMat3Uniform(const std::shared_ptr<GLProgram>& program, std::string name)
{
    _location = glGetUniformLocation(program->get_program_ID(), name.data());
}

void GLMat3Uniform::activate()
{
    glUniformMatrix3fv(_location, 1, GL_FALSE, glm::value_ptr(value));
}

GLVec2Uniform::GLVec2Uniform(const std::shared_ptr<GLProgram>& program, std::string name)
{
    _location = glGetUniformLocation(program->get_program_ID(), name.data());
}

void GLVec2Uniform::activate()
{
    glUniform2f(_location, value.x, value.y);
}