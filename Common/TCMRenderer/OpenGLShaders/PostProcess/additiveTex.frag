#version 430 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;

layout(location = 14) uniform float weight;

void main()
{
	vec3 screenColor = texture(screenTexture, TexCoords).rgb;
	vec3 bloomColor = texture(bloomTexture, TexCoords).rgb;
	
	color = vec4(screenColor + bloomColor * weight, 1.0);
}