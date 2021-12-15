#include <GL/GLDrawProgram.hpp>
#include <fstream>
#include <GL/GLProgram.hpp>
#include <string>

std::string read_from_file(std::string file_path)
{
	std::ifstream shader(file_path);
	std::string str_shader, line;
	while (getline(shader, line))
	{
		str_shader += line + "\n";
	}
	shader.close();
    return str_shader;
}

GLDrawProgram::GLDrawProgram()
    : GLProgram(read_from_file("../../../../SpaceIntruders/ImaginaryEngine/src/shader_vert.vert"), 
				read_from_file("../../../../SpaceIntruders/ImaginaryEngine/src/shader_frag.frag"))
{

}