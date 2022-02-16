PS_IN vec2 oTexCoord;
PS_IN vec4 oColor;
uniform sampler2D uTexture;
void main()
{
	PS_OUT = TEXTURE2D(uTexture, oTexCoord) * oColor;
}