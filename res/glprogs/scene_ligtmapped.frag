#version 330
out vec4 FragColor;
  
uniform sampler2D diffuse;
uniform sampler2D lightmap;

in vec2 oTexCoord;
in vec2 oLMCoord;

void main()
{
	vec4 lmSample = texture2D(lightmap,oLMCoord);
	vec4 diffuseSample = texture2D(diffuse,oTexCoord);

    FragColor = lmSample * diffuseSample;
 //   FragColor = lmSample;
   // FragColor = diffuseSample;

	//FragColor = vec4(oLMCoord.x,oLMCoord.y,0,1);
} 