#version 330


layout(location = 0) out vec4 oFragColor;
layout(location = 1) out int oSelColor;
  
uniform sampler2D diffuse;
uniform sampler2D lightmap;

in vec2 oTexCoord;
in vec2 oLMCoord;

void main()
{
	vec4 lmSample = texture2D(lightmap,oLMCoord);
	vec4 diffuseSample = texture2D(diffuse,oTexCoord);

    oFragColor = vec4(lmSample.rgb,1) *  diffuseSample;	
				oSelColor = 0;
} 