#ifdef VERTEX_SHADER

#include "common_vertex.h"

uniform vec4 u_Color;
uniform vec4 u_Color2;

out vec4 oVertexColor;

void main()
{
	oVertexColor = mix(u_Color, u_Color2, color.x);
	gl_Position = vec4(xyz,1);	    
}

#endif

#ifdef FRAGMENT_SHADER

out vec4 oFragColor;
in vec4 oVertexColor;

void main()
{
	oFragColor = oVertexColor;	     	
} 


#endif