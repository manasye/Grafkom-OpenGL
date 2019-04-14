#version 130

in vec3 vertColor;
in vec2 texCoord;

uniform sampler2D thisTexture;

out vec4 FragColor;

void main()
{
    if (texCoord == vec2(-999, -999)) {
        FragColor = vec4(vertColor, 1.0);
    } else {
        FragColor = texture(thisTexture, texCoord);
    }
}
