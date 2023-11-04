#version 330
out vec4 FragColor;

uniform vec4 groupColor;

in vec2 oTexCoord;
in vec2 oLMCoord;

void main()
{		
    FragColor = groupColor;    
} 