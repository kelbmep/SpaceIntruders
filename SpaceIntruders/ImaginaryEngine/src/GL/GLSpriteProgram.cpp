#include <GL/GLSpriteProgram.hpp>
#include <string>
#include <engine.hpp>
#include <fileManager.hpp>

GLSpriteProgram::GLSpriteProgram(const Engine& engine)
    : GLProgram({"position", "texCoord", "color"},
                engine.get_file_manager().resource_location("shader_vert.vert"),
				engine.get_file_manager().resource_location("shader_frag.frag"))
{
}