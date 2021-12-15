#version 330 core
in vec2 oTexCoord;
in vec4 oColor;
out vec4 color;
uniform sampler2D uTexture;
void main()
{
	color = texture(uTexture, oTexCoord) * oColor;
}