#version 330
out vec4 FragColor;
  
uniform sampler2D diffuse;
uniform sampler2D lightmap;

in vec2 oTexCoord;

void main()
{
	vec4 lmSample = texture2D(lightmap,oTexCoord);
	vec4 diffuseSample = texture2D(diffuse,oTexCoord);

    FragColor = lmSample * diffuseSample;
//    FragColor = diffuseSample;
} 