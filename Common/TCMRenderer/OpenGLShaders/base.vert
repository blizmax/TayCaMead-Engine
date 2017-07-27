#version 430

in vec4	a_Position;  // x,y,z
in vec3	a_Normal;	 // NX, NY, NZ
in vec2	a_TexCoords; // U, V
in vec3	a_Tangent;	 // NX, NY, NZ

out	VertexFragmentData
{
	vec2	v_TexCoords;
	vec3	v_UpdatedNormals;
	vec4	v_TruePosition;
	vec3	v_Tangent;
	vec3	v_ToTheCamRay;
	vec4	v_PosLightSpace;
}	OUT;

layout(location = 9) uniform mat4 	u_WorldMatrix;
layout(location = 10) uniform float	u_Time;
layout(location = 11) uniform vec3	u_CameraPosition;

layout(std140)	uniform s_light
{
	mat4	lightSpace;
	vec4	direction;
	vec3	position;
	float	constantAttenuation;

	vec3	ambientColor;
	float	linearAttenuation;

	vec3	diffuseColor;
	float	quadraticAttenuation;

	vec3	specularColor;
	
	float cutOff;
    float outerCutOff;
} light;

layout(std140)	uniform ViewProjectionMatrix
{
	mat4	u_ViewMatrix;
	mat4	u_ProjectionMatrix;
};

void main(void)
{
	OUT.v_TexCoords = a_TexCoords;
	OUT.v_Tangent = a_Tangent;
	
	OUT.v_UpdatedNormals = vec3(normalize(u_WorldMatrix * vec4(a_Normal, 0.0)));
	OUT.v_TruePosition = u_WorldMatrix * a_Position;
	OUT.v_ToTheCamRay = normalize(u_CameraPosition - OUT.v_TruePosition.xyz);

	OUT.v_PosLightSpace = light.lightSpace * OUT.v_TruePosition;

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_WorldMatrix * a_Position;
}
