// Version by default
// #version 330

#ifdef VERTEX_SHADER

#include "common_vertex.h"

out vec2 oTexCoord;
out vec2 oLMCoord;
out vec3 oNormal;

void main()
{
	oTexCoord  = uv.xy;
	oLMCoord  = color.xy;

	gl_Position = _transform();	
}

#endif


#ifdef FRAGMENT_SHADER

#include "common_fragment.h"

uniform int u_RenderMode;

// Solid color (for point entities)
uniform vec4 u_Color;

// Diffuse
uniform sampler2D diffuse;
in vec2 oTexCoord;

// Lightmap
uniform sampler2D lightmap;
in vec2 oLMCoord;

// Normal color (for flatshaded)
in vec3 oNormal;



void main()
{				
	switch(u_RenderMode)
	{
		case RM_LIGHTSHADED:
		{
			vec4 diffuseSample = texture2D(diffuse,oTexCoord);	
			vec4 lmSample = texture2D(lightmap,oLMCoord);
			oFragColor = diffuseSample * lmSample;			
		}		
		break;
		case RM_UNSHADED:
		{
			vec4 diffuseSample = texture2D(diffuse,oTexCoord);			
			oFragColor = diffuseSample;			
		}							
		break;
		case RM_FLATSHADED:		
			oFragColor = vec4(oNormal,1);						
		break;
		case RM_SOLID_COLOR:
			oFragColor = u_Color;
			break;	
		case RM_WIREFRAME:
			// * implement *
		break;
	}
  
	oSelColor = u_ObjectSerialNumber;  
} 

#endif