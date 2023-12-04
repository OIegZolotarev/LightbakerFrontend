// Version by default
// #version 330

#ifdef VERTEX_SHADER

#include "common_vertex.h"

#ifdef DIFFUSEMAP
out vec2 oTexCoord;
#endif

#ifdef LIGHTMAP
out vec2 oLMCoord;
#endif

void main()
{

#ifdef DIFFUSEMAP
	oTexCoord  = uv.xy;
#endif
	
#ifdef LIGHTMAP	
	oLMCoord  = color.xy;
#endif

	gl_Position = projection * (view * (transform * vec4(xyz * scale, 1.0)));	
}

#endif


#ifdef FRAGMENT_SHADER

#include "common_fragment.h"
  
#ifdef DIFFUSEMAP
uniform sampler2D diffuse;
in vec2 oTexCoord;
#endif 

#ifdef LIGHTMAP	
uniform sampler2D lightmap;
in vec2 oLMCoord;
#endif

void main()
{
	FragColor = vec4(1,1,1,1);
			
	#ifdef DIFFUSEMAP
	vec4 diffuseSample = texture2D(diffuse,oTexCoord);
	FragColor = diffuseSample;
	#endif
				
	#ifdef LIGHTMAP
	vec4 lmSample = texture2D(lightmap,oLMCoord);
	FragColor *= lmSample;
	#endif
  
	oSelColor = u_ObjectSerialNumber;
  
} 

#endif