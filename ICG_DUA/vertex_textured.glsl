attribute vec4 attr_position;
attribute vec4 attr_color;
attribute vec2 attr_texCoord;

varying vec4 varyingColor;
varying vec2 varyingTexCoord;

void main()
{
	varyingColor = attr_color;
	varyingTexCoord = attr_texCoord;
	gl_Position = attr_position;
}