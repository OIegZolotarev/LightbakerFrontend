#version 330

layout (location = 0) in vec3 xyz; 
layout (location = 1) in vec3 normal; 
layout (location = 2) in vec3 tangent; 
layout (location = 3) in vec4 color; 
layout (location = 4) in vec3 uv; 

uniform vec3 scale;
uniform mat4 view;
uniform mat4 projection;

out vec2 oTexCoord;
out vec2 oLMCoord;

void main()
{
	oTexCoord  = uv.xy;
	oLMCoord  = color.xy;
    gl_Position = projection * (view * vec4(xyz * scale, 1.0));	    
}