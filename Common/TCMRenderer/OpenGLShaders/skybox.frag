#version 330 core
in vec3 TexCoords;
layout (location = 0) out vec4 o_FragColor;
layout (location = 1) out vec4 o_BrightColor;

uniform samplerCube skybox;

void main()
{    
    o_FragColor = texture(skybox, TexCoords);
}