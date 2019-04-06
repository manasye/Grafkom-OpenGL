#version 130

in vec3 position;
in vec3 inColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertColor;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    vertColor = inColor;
}
