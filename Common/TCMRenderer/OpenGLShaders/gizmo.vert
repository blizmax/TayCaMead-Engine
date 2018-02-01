#version 330 core

in vec4	a_Position;  // x,y,z
in vec4	a_Color;  // x,y,z

out vec4 v_Color;

layout (location = 10) uniform mat4 u_mvp;

void main(void)
{
	v_Color = a_Color;
	gl_Position = u_mvp * a_Position;
}