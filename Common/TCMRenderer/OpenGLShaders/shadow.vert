#version 430 core
in vec4	a_Position;  // x,y,z
in vec3	a_Normal;	 // NX, NY, NZ
in vec2	a_TexCoords; // U, V
in vec3	a_Tangent;	 // NX, NY, NZ

layout(std140, binding = 1)	uniform WorldTranformMatrix
{
	mat4	u_WorldMatrix;
	mat4	u_TransformationMatrix;
};

layout(std140, binding = 3)	uniform s_light
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

void main()
{
    gl_Position = light.lightSpace * u_WorldMatrix * a_Position;
}  