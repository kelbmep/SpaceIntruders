#include <GL/GLProgram.hpp>
#include <GL/GLTexture.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/GLHeaders.hpp>
#include <iostream>
#include <string>

GLProgram::GLProgram(/*std::initializer_list<const char*> attributes, */std::string vs_s, std::string ps_s)
{/*
    std::string vs_header =
#if GLES20
R"(
#version 100
#define VS_IN attribute;
#define VS_OUT varying;
    )";
#elif GL33
    R"(
#version 330
#define VS_IN in;
#define VS_OUT out;
    )";
#endif
    */
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
    /*
    std::string ps_header =
#if GLES20
R"(
#version 100
#precision medium float
#define PS_IN varying;
#define PS_OUT gl_FragColor;
#define TEXTURE2D texture;
    )";
#elif GL33

#endif
*/
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

  //  size_t index = 0;


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
}

std::shared_ptr<TextureUniform> GLProgram::create_texture_uniform(std::string name)
{
    return std::make_shared<GLTextureUniform>(std::static_pointer_cast<GLProgram>(shared_from_this()), name);
}

std::shared_ptr<Mat3Uniform> GLProgram::create_mat3_uniform(std::string name)
{
    return std::make_shared<GLMat3Uniform>(std::static_pointer_cast<GLProgram>(shared_from_this()), name);
}

std::shared_ptr<Vec2Uniform> GLProgram::create_vec2_uniform(std::string name)
{
    return std::make_shared<GLVec2Uniform>(std::static_pointer_cast<GLProgram>(shared_from_this()), name);
}

std::shared_ptr<Vec3Uniform> GLProgram::create_vec3_uniform(std::string name)
{
    return std::make_shared<GLVec3Uniform>(std::static_pointer_cast<GLProgram>(shared_from_this()), name);
}

std::shared_ptr<FloatUniform> GLProgram::create_float_uniform(std::string name)
{
    return std::make_shared<GLFloatUniform>(std::static_pointer_cast<GLProgram>(shared_from_this()), name);
}

void GLTextureUniform::activate()
{
    Uniform::activate();
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
    _location = glGetUniformLocation(program->get_program_ID(), name.c_str());
}

GLMat3Uniform::GLMat3Uniform(const std::shared_ptr<GLProgram>& program, std::string name)
{
    _location = glGetUniformLocation(program->get_program_ID(), name.c_str());
}

void GLMat3Uniform::activate()
{
    Uniform::activate();
    glUniformMatrix3fv(_location, 1, GL_FALSE, glm::value_ptr(value));
}

GLVec2Uniform::GLVec2Uniform(const std::shared_ptr<GLProgram>& program, std::string name)
{
    _location = glGetUniformLocation(program->get_program_ID(), name.c_str());
}

void GLVec2Uniform::activate()
{
    Uniform::activate();
    glUniform2f(_location, value.x, value.y);
}

GLVec3Uniform::GLVec3Uniform(const std::shared_ptr<GLProgram>& program, std::string name)
{
    _location = glGetUniformLocation(program->get_program_ID(), name.c_str());
}

void GLVec3Uniform::activate()
{
    Uniform::activate();
    glUniform3f(_location, value.x, value.y, value.z);
}

GLFloatUniform::GLFloatUniform(const std::shared_ptr<GLProgram>& program, std::string name)
{
    _location = glGetUniformLocation(program->get_program_ID(), name.c_str());
}

void GLFloatUniform::activate()
{
    Uniform::activate();
    glUniform1f(_location, value);
}

void GLTextureUniform::activate_with_slot(size_t slot)
{
    Uniform::activate();
    auto glTexture = std::dynamic_pointer_cast<GLTexture>(texture);
    if (glTexture)
    {
        //TODO: diff texture slots
        glActiveTexture(GL_TEXTURE0 + slot);
        glTexture->active();

        glUniform1i(_location, slot);
    }
}