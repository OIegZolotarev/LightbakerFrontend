#pragma once
#include "common.h"
#include "gl_backend.h"

namespace DrawUtils
{
	DrawMesh* MakeWireframeBox(glm::vec3 extents = glm::vec3(1, 1, 1));
	DrawMesh* MakeWireframeSphere(float detail = 36,float r = 1);
}