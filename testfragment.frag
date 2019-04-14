#version 130

in vec3 vertColor;
in vec2 texCoord;

uniform sampler2D thisTexture;

out vec4 FragColor;

void main()
{
    FragColor = texture(thisTexture, texCoord);
}
