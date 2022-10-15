/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once
#include "gl_texture.h"

enum class CommonIcons
{
	LoadFile = 0,
	SpotLight,
	DirectLight,
	OmniLight,
	Bake
};

void			InitCommonResources();
gltexture_t*	GetCommonIcon(CommonIcons icon_id);