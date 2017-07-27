#version 430 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D screenTexture;

layout(location = 14) uniform float	exposure;

void main()
{             
    const float gamma = 2.2;
    vec3 color = texture(screenTexture, TexCoords).rgb;      
    vec3 result = vec3(1.0) - exp(-color * exposure);
    FragColor = vec4(result , 1.0f);
}