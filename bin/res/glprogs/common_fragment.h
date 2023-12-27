// Common routines for scene objects rendering and selection

uniform int u_ObjectSerialNumber;

layout(location = 0) out vec4 oFragColor;
layout(location = 1) out int oSelColor;

#define RM_LIGHTSHADED  0 // Diffuse + Lightmap
#define RM_UNSHADED 1 // Diffuse
#define RM_FLATSHADED 2 // Normal 
#define RM_WIREFRAME 3 
#define RM_SOLID_COLOR 4