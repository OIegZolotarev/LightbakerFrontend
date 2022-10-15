/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "draw_utils.h"

DrawMesh* DrawUtils::MakeWireframeBox(glm::vec3 extents)
{
	DrawMesh* pResult = new DrawMesh(DrawMeshFlags::NoColor | DrawMeshFlags::NoUV);

	glm::vec3 max = extents * 0.5f;
	glm::vec3 min = extents * -0.5f;

	pResult->Begin(GL_LINES);

	// Bottom plane

	pResult->Vertex3f(min.x, min.y, min.z);
	pResult->Vertex3f(max.x, min.y, min.z);

	pResult->Vertex3f(max.x, min.y, min.z);
	pResult->Vertex3f(max.x, max.y, min.z);

	pResult->Vertex3f(max.x, max.y, min.z);
	pResult->Vertex3f(min.x, max.y, min.z);

	pResult->Vertex3f(min.x, max.y, min.z);
	pResult->Vertex3f(min.x, min.y, min.z);

	// Top plane

	pResult->Vertex3f(min.x, min.y, max.z);
	pResult->Vertex3f(max.x, min.y, max.z);

	pResult->Vertex3f(max.x, min.y, max.z);
	pResult->Vertex3f(max.x, max.y, max.z);

	pResult->Vertex3f(max.x, max.y, max.z);
	pResult->Vertex3f(min.x, max.y, max.z);

	pResult->Vertex3f(min.x, max.y, max.z);
	pResult->Vertex3f(min.x, min.y, max.z);

	// Sides

	pResult->Vertex3f(min.x, min.y, max.z);
	pResult->Vertex3f(min.x, min.y, min.z);

	pResult->Vertex3f(max.x, min.y, max.z);
	pResult->Vertex3f(max.x, min.y, min.z);

	pResult->Vertex3f(max.x, max.y, max.z);
	pResult->Vertex3f(max.x, max.y, min.z);

	pResult->Vertex3f(min.x, max.y, max.z);
	pResult->Vertex3f(min.x, max.y, min.z);
	
	pResult->End();

	return pResult;
}

DrawMesh* DrawUtils::MakeWireframeSphere(float detail,float r)
{
	DrawMesh* pResult = new DrawMesh(DrawMeshFlags::NoColor | DrawMeshFlags::NoUV);

	pResult->Begin(GL_LINE_STRIP);

	// Horizontal plane

	int step = 360 / detail;

	int i = 0;

	for (i = 0; i <= 360; i += step)
	{
		float a = (i / 360.f) * 6.28f;

		glm::vec3 pt;
		pt.x = cos(a) * r / 2;
		pt.z = sin(a) * r / 2;
		pt.y = 0;

		pResult->Vertex3f(pt.x,pt.y,pt.z);
	}

	for (;i >= 0; i -= step)
	{
		float a = (i / 360.f) * 6.28f;

		glm::vec3 pt;
		pt.x = cos(a) * r / 2;
		pt.y = sin(a) * r / 2;
		pt.z = 0;

		pResult->Vertex3f(pt.x, pt.y, pt.z);
	}

	pResult->End();

	return pResult;
}
