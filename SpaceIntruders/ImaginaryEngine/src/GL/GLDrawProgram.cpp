#include <GL/GLDrawProgram.hpp>
//#include <GL/GlHeaders.hpp>
#include <fstream>
#include <GL/GLProgram.hpp>
#include <string>
//#include <iostream>

std::string read_from_file(std::string file_path)
{
	std::ifstream shader(file_path);
	std::string str_shader, line;
	while (getline(shader, line))
	{
		str_shader += line + "\n";
	}
	shader.close();
    //std::cout << str_shader;
	return str_shader;
}
/*
namespace
{
    constexpr auto vs_program =
        R"(
#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

out vec2 oTexCoord;

uniform vec2 uScreenSize;
uniform mat3 uTransform;


void main()
{
    vec2 pos = (uTransform * vec3(position, 1.0)).xy;
    vec2 scaledPos = pos / uScreenSize;
    scaledPos.y = 1.0 - scaledPos.y;
    scaledPos = scaledPos * 2.0 - vec2(1.0);

    oTexCoord = texCoord;
    gl_Position = vec4(scaledPos.x, scaledPos.y, 1.0, 1.0);
}
)";
    constexpr auto ps_program =
        R"(
#version 330 core

uniform sampler2D uTexture;

in vec2 oTexCoord;
out vec4 color;

void main()
{
    color = texture(uTexture, oTexCoord);
}
)";
}*/

GLDrawProgram::GLDrawProgram()
    : GLProgram(read_from_file("../../../../SpaceIntruders/ImaginaryEngine/src/shader_vert.vert"), 
				read_from_file("../../../../SpaceIntruders/ImaginaryEngine/src/shader_frag.frag"))
{

}