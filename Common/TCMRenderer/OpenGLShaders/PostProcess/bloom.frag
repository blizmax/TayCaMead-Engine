#version 430 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;

void main()
{
	vec3 result = texture(screenTexture, TexCoords).rgb;
	float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
	color = vec4(result * brightness, 1.0);
}