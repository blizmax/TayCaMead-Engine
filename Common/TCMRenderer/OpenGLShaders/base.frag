#version 430

in	VertexFragmentData
{
	vec2	v_TexCoords;
	vec3	v_UpdatedNormals;
	vec4	v_TruePosition;
	vec3	v_ToTheCamRay;
}	IN;

layout (location = 0) out vec4 o_FragColor;

layout (location = 14) uniform vec4 color;

void main(void)
{
	o_FragColor = vec4(color.xyz, 1.0);
}
