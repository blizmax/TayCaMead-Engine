// INPUTS du Fragment Shader = INPUTS du Vertex Shader

#version 430

in	VertexFragmentData
{
	vec2	v_TexCoords;
	vec3	v_UpdatedNormals;
	vec4	v_TruePosition;
	vec3	v_Tangent;
	vec3	v_ToTheCamRay;
	vec4	v_PosLightSpace;
}	IN;

layout (location = 0) out vec4 o_FragColor;

layout(location = 10) uniform float	u_Time;

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

layout (std140) uniform MaterialBlock
{
	vec3	AmbientColor;
	float	AmbientMapEnabled;

	vec3	DiffuseColor;
	float	DiffuseMapEnabled;

	vec3	SpecularColor;
	float	SpecularMapEnabled;

	vec3	EmissiveColor;
	float	EmissiveMapEnabled;

	vec3	TransmissionFilter;
	float	OpticalDensity;
	float	Shininess;
	float	ShininessMapEnabled;

	float	Alpha;
	float	AlphaMapEnabled;

	float	BumpMapEnabled;
	
	//int		LightParam;
	int		Sharpness;
} material;

uniform sampler2D AmbientMap;
uniform sampler2D DiffuseMap;
uniform sampler2D SpecularColorMap;
uniform sampler2D ShininessMap;
//uniform sampler2D EmissiveMap;
uniform sampler2D BumpMap;
uniform sampler2D AlphaMap;

uniform sampler2D ShadowMap;
layout (location = 13) uniform bool	  ShadowEnabled;


float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(ShadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(IN.v_UpdatedNormals);
    vec3 lightDir = normalize(light.position.xyz - IN.v_TruePosition.xyz);
    float bias = max(0.05 * (1.0 - dot(normal, light.direction.xyz)), 0.005);
    // Check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main(void)

{
	
	/////////////////////////////////////////

	// Set basic diffuse directionnal light: Direction, Color and Specular Color
	/*light.direction = vec4(1, -0.3, -0.5, 0);
	light.ambientColor = vec3(1.0, 1.0, 1.0);
	light.diffuseColor = vec3(1.0, 1.0, 1.0);
	light.specularColor = vec3(0.5, 0.5, 0.5);*/

	///////////////////////////////////////////
	
	// Get texture color
	vec2 texCoords = vec2(IN.v_TexCoords.x, 1.0 - IN.v_TexCoords.y);

	vec3 Normal = normalize(IN.v_UpdatedNormals);

	if (material.BumpMapEnabled == 1.0f)
	{
		// defini une matrice de rotation Tangent-Space -> Local Space
		vec3 T = normalize(IN.v_Tangent);
		vec3 N = Normal;
		vec3 B = cross(N, T);// bitangent perpendiculaire au plan <T,N>

		mat3 TBN = mat3(T, B, N);

		vec3 bump = texture(BumpMap, texCoords).rgb * 2.0 - 1.0;
		//vec3 N = normalize(bump);  // suffisant si texture en object space
		// convertie la normale de tangent-space (texture) vers object space
		Normal = normalize(TBN * bump);
	}


	////////////////////

	// Set diffuse color
	float	angleToLight = dot(normalize(-light.direction).xyz, Normal); // Get angle to light
	float	I = max(angleToLight, 0.0); // Get Intensity

	vec3	finalDiffuse = I * light.diffuseColor * material.DiffuseColor; // Get diffuse 
	if (material.DiffuseMapEnabled == 1.0f)
		finalDiffuse = texture2D(DiffuseMap, texCoords).xyz; 
	////////////////////

	// Set Specular light
	vec3	reflectedRay = reflect(normalize(light.direction).xyz, Normal); // Get reflected light ray

	bool	isFrontFace = dot(IN.v_ToTheCamRay, Normal) > 0.0; // If we are seeing the "front face" or not

	vec3	finalSpecular = vec3(0.0, 0.0, 0.0);

	if (isFrontFace)
	{
		float Shininess = material.Shininess;
		if (material.ShininessMapEnabled == 1.0f)
			Shininess *= texture2D(ShininessMap, texCoords).x;

		// Calculate specular color
		finalSpecular = light.specularColor * material.SpecularColor 
						* pow(max(dot(IN.v_ToTheCamRay, reflectedRay), 0.0), Shininess);

		if (material.SpecularMapEnabled == 1.0f)
			finalSpecular *= texture2D(SpecularColorMap, texCoords).xzy;
	}

	/////////////////////

	vec3	finalAmbient = light.ambientColor * material.AmbientColor;

	if (material.AmbientMapEnabled == 1.0f)
		finalAmbient *= texture2D(AmbientMap, texCoords).xyz;
	else if ( material.DiffuseMapEnabled == 1.0f)
		finalAmbient *= texture2D(DiffuseMap, texCoords).xyz;

	
	vec3 finalEmissive = material.EmissiveColor;
	//if (material.EmissiveMapEnabled == 1.0f)
	//	finalEmissive *= texture2D(EmissiveMap, texCoords).xyz;
	
	////////////////////////////////////// 
	float mainAttenuation = 1.0;
	
	if (light.direction.w == 1.0)
	{
		float distanceFromObjectToLight = length(light.position - IN.v_TruePosition.xyz);
		mainAttenuation = 1.0 / (light.constantAttenuation) + (light.linearAttenuation * distanceFromObjectToLight) + pow(light.quadraticAttenuation * distanceFromObjectToLight, 2.0); 

		float epsilon = (light.cutOff - light.outerCutOff);
		if (epsilon != 0)
		{
			float intensity = clamp((angleToLight - light.outerCutOff) / epsilon, 0.0, 1.0);
			finalDiffuse  *= intensity;
			finalSpecular *= intensity;
		}
	}
	
	float Alpha = material.Alpha;
	if (material.AlphaMapEnabled == 1.0f)
		Alpha *= texture2D(AlphaMap, texCoords).x;

	//o_FragColor = vec4 (texture2D(DiffuseMap, texCoords).xyz, 1.0);
	
	float shadow = ShadowEnabled ? ShadowCalculation(IN.v_PosLightSpace) : 0.0;                      
    shadow = min(shadow, 0.75); // reduce shadow strength a little: allow some diffuse/specular light in shadowed regions
    o_FragColor = vec4(finalEmissive.xyz + finalAmbient.xyz + (1.0 - shadow) * ((finalDiffuse.xyz + finalSpecular.xyz) * mainAttenuation).xyz, Alpha);
}