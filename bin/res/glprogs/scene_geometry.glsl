// Version by default
// #version 330

#ifdef VERTEX_SHADER

#include "common_vertex.h"

#ifdef DIFFUSE
out vec2 oTexCoord;
#endif 

#ifdef LIGHTMAP
out vec2 oLMCoord;
#endif 

#ifdef FLATSHADED
out vec3 oNormal;
#endif

void main()
{

#ifdef DIFFUSE
	oTexCoord  = uv.xy;
#endif

#ifdef LIGHTMAP
	oLMCoord  = color.xy;
#endif

#ifdef NORMAL
	oNormal = normal;
#endif

#ifdef USING_BONES 
	vec4 pos = vec4(xyz,1) * u_BonesTransform[bones[0]];
	gl_Position = _transform(pos.xyz);	
#else
	gl_Position = _transform();	
#endif 

}

#endif


#ifdef FRAGMENT_SHADER

#include "common_fragment.h"


#ifdef SOLID_COLOR
// Solid color (for point entities)
uniform vec4 u_Color;
#endif

#ifdef DIFFUSE
// Diffuse
uniform sampler2D diffuse;
in vec2 oTexCoord;
#endif

#ifdef LIGHTMAP
// Lightmap
uniform sampler2D lightmap;
in vec2 oLMCoord;
#endif

#ifdef FLATSHADED
// Normal color (for flatshaded)
in vec3 oNormal;
#endif

void main()
{		
	vec4 oFragcolor = vec4(1,1,1,1)	;

#ifdef DIFFUSE
		vec4 diffuseSample = texture2D(diffuse,oTexCoord);	
		oFragColor *= diffuseSample;
#endif

#ifdef LIGHTMAP
		vec4 lmSample = texture2D(lightmap,oLMCoord);
		oFragColor *= lmSample;
#endif

#ifdef NORMAL
	oFragColor *= oNormal;
#endif

#ifdef SOLID_COLOR
	oFragColor *= u_Color;
#endif

	oSelColor = u_ObjectSerialNumber;  
} 

#endif