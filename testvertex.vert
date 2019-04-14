#version 130

in vec3 position;
in vec3 inColor;
in vec2 textureCoordinate;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertColor;
out vec2 texCoord;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    vertColor = inColor;
    texCoord = textureCoordinate;
}
