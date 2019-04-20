#version 130

in vec3 vertColor;
in vec2 texCoord;
in vec3 norms;
in vec3 pos;

uniform sampler2D thisTexture;
uniform vec3 lightPos0;

out vec4 FragColor;

void main()
{
    //Ambient light
    vec3 ambientLight = vec3(0.8f, 0.8f, 0.8f);

    //Diffuse light
    vec3 positionToLight = normalize(pos - lightPos0);
    vec3 diffuseColor = vec3(1.f,1.f,1.f); // we want light is white color
    float diffuse = clamp(dot(positionToLight, norms), 0, 1);
    vec3 diffuseFinal = diffuseColor * diffuse;

    if (texCoord == vec2(-999, -999)) {
        FragColor = vec4(vertColor, 1.0);
    } else {
        FragColor = texture(thisTexture, texCoord)
                    * vec4(ambientLight, 1.f)
                    + vec4(diffuseFinal, 1.f);
    }
}
