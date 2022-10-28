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

DrawMesh* DrawUtils::MakeWireframeCone(float detail /*= 36*/, float r /*= 1*/, float h /*= 1*/)
{
	DrawMesh* pResult = new DrawMesh(DrawMeshFlags::NoColor | DrawMeshFlags::NoUV);

	pResult->Begin(GL_LINES);

	int step = 360 / detail;

	int i = 0;



	for (i = step; i <= 360; i += step)
	{
		float a = (i / 360.f) * 6.28f;
		float b = ((i - step) / 360.f) * 6.28f;

		glm::vec3 ptA, ptB;

		pResult->TexCoord2f(1, 1);

		ptA.x = cos(a) * r / 2;
		ptA.y = sin(a) * r / 2;
		ptA.z = -h;


		ptB.x = cos(b) * r / 2;
		ptB.y = sin(b) * r / 2;
		ptB.z = -h;

		// Расширяемый сегмент

		pResult->TexCoord2f(1, 1);
		pResult->Vertex3f(ptA.x, ptA.y, ptA.z);
		pResult->Vertex3f(ptB.x, ptB.y, ptB.z);


		// Линия 1
		pResult->TexCoord2f(1, 1);
		pResult->Vertex3f(ptA.x, ptA.y, ptA.z);

		pResult->TexCoord2f(0, 0);
		pResult->Vertex3f(0, 0, 0);

		// Линия 2
		pResult->TexCoord2f(1, 1);
		pResult->Vertex3f(ptB.x, ptB.y, ptB.z);

		pResult->TexCoord2f(0, 0);
		pResult->Vertex3f(0, 0, 0);

	}

	pResult->End();

	return pResult;
}


