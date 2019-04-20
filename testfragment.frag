#version 130

in vec3 vertColor;
in vec2 texCoord;
in vec3 norms;

uniform sampler2D thisTexture;
uniform vec3 lightPos0;

out vec4 FragColor;

void main()
{
    //Ambient light
    vec3 ambientLight = vec3(0.1f, 0.1f, 0.1f);

    if (texCoord == vec2(-999, -999)) {
        FragColor = vec4(vertColor, 1.0);
    } else {
        FragColor = texture(thisTexture, texCoord) * vec4(ambientLight, 1.f);
    }
}
