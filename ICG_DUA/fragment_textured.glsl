varying vec4 varyingColor;
varying vec2 varyingTexCoord;
uniform sampler2D uniform_Texture;

void main() {
    gl_FragColor = texture2D(uniform_Texture, varyingTexCoord) * varyingColor;
}