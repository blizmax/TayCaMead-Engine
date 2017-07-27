#version 430
layout (location = 0) in vec3 position;
out vec3 TexCoords;

layout (location = 2) uniform mat4 projection; 
layout (location = 3) uniform mat4 view; 


void main()
{
    vec4 pos = projection * view * vec4(position, 1.0); 
    gl_Position = pos.xyww;
    TexCoords = position;
}  