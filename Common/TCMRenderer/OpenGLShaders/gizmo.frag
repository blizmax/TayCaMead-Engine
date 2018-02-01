#version 330 core

in vec4 v_Color;

layout (location = 0) out vec4 o_FragColor;

void main(void)
{
	o_FragColor = vec4(v_Color.xyz, 1.0);
}