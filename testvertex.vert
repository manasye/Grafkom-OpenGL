#version 130

in vec3 position;
in vec3 inColor;
in vec2 textureCoordinate;
in vec3 normals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertColor;
out vec2 texCoord;
out vec3 norms;
out vec3 pos;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    pos = vec4(model * vec4(position, 1.f)).xyz;
    vertColor = inColor;
    texCoord = textureCoordinate;
    norms = mat3(model) * normals;
}
