#version 330 core
in vec3 oCol;
out vec4 color;
void main()
{
	color = vec4(oCol, 1.0);
}