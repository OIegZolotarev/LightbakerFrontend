#version 330

layout (location = 0) in vec3 xyz; 
layout (location = 1) in vec3 normal; 
layout (location = 2) in vec3 tangent; 
layout (location = 3) in vec4 color; 
layout (location = 4) in vec3 uv; 
  
out vec4 vertexColor; // specify a color output to the fragment shader

uniform	vec4 colorOverride;
uniform vec3 scale;
uniform vec3 origin;
uniform mat4 view;
uniform mat4 transform;
uniform mat4 projection;

void main()
{
    gl_Position = projection * (view * (transform * vec4(xyz * scale, 1.0)));	
    vertexColor = colorOverride;    
}