#version  330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec3 col;
out vec3 oCol;
vec2 oPos;
uniform vec2 screenSize;
void main()
{
	oPos = position / screenSize * 2 - 1;
	gl_Position = vec4(oPos.x, oPos.y, 1.0, 1.0);
	oCol = col;
}