#pragma once
#include "gl_texture.h"

// Light flags from LB3k app

#define LF_EULER	(1<<0)
#define LF_DYN		(1<<1)
#define LF_XYZ		(1<<2)
#define LF_LINEAR	(1<<3)
#define LF_DISK		(1<<4)
#define LF_RECT		(1<<5)

enum class LightTypes
{
	Omni = 0,
	Spot,
	Direct
};

typedef struct lightDef_s
{
	LightTypes	type = LightTypes::Omni;
	int			flags = 0;

	glm::vec3	pos;
	glm::vec3	color;

	float		intensity = 1;

	glm::vec3	anglesDirection;
	float		cones[2];
	glm::vec2	size;

	int			style = 0;

	gltexture_s* editor_icon;

}lightDef_t;


