/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#include "application.h"
#include "mod_obj_asynch_exporter.h"

void ModObjAsynchExporter::ExportLightDefs() const
{
	auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();
	float scale = 1.0f / sceneRenderer->GetSceneScale();

	fprintf(m_pFPOut, "### LightBaker 3000 lights definitions\n\n");

	fprintf(m_pFPOut, "#scene_scale %.3f\n\n", sceneRenderer->GetSceneScale());

	fprintf(m_pFPOut, "#lm_size %d %d\n", m_pData->lightmapDimensions[0], m_pData->lightmapDimensions[1]);
	fprintf(m_pFPOut, "#env_color %.3f %.3f %.3f\n\n", m_pData->envColor[0], m_pData->envColor[1], m_pData->envColor[1]);

	fprintf(m_pFPOut, "#lights_start\n");

	for (auto& it : m_pData->lightDefs)
	{
		std::string lightBaseType;

		switch (it.type)
		{
		case LightTypes::Omni:
			lightBaseType = "#omni";
			break;
		case LightTypes::Spot:
			lightBaseType = "#spot";
			break;
		case LightTypes::Direct:
			lightBaseType = "#direct";
			break;
		default:
			Application::EPICFAIL("Unknown light type %d !", it.type);
			break;
		}

		if (it.flags & LF_EULER)	lightBaseType += "_euler";
		if (it.flags & LF_DYN)		lightBaseType += "_dyn";
		if (it.flags & LF_XYZ)		lightBaseType += "_xyz";
		if (it.flags & LF_LINEAR)	lightBaseType += "_linear";
		if (it.flags & LF_DISK)		lightBaseType += "_disk";
		if (it.flags & LF_RECT)		lightBaseType += "_rect";


		fprintf(m_pFPOut, "%s\t%.6f %.6f %.6f\t%.6f %.6f %.6f\t%.6f\t%.6f %.3f %.6f\t%.6f %.6f\t%d\t",
			lightBaseType.c_str(),
			it.pos[0] * scale, it.pos[1] * scale, it.pos[2] * scale,
			it.color[0], it.color[1], it.color[2],
			it.intensity * scale,
			it.anglesDirection[0], it.anglesDirection[1], it.anglesDirection[2],
			it.cones[0], it.cones[1],
			it.style);

		switch (it.type)
		{
		case LightTypes::Omni:
		case LightTypes::Spot:
			if (it.flags & LF_RECT)
			{
				fprintf(m_pFPOut, "%.3f %.3f\n", it.size[0], it.size[1]);
			}
			else
				fprintf(m_pFPOut, "%.3f\n", it.size[0]);

			break;
		case LightTypes::Direct:
			fprintf(m_pFPOut, "\n");
			break;
		default:
			break;

		}
	}

	fprintf(m_pFPOut, "#lights_end\n");
}

void ModObjAsynchExporter::ExportVerticles() const
{
	size_t vertsSize = m_pData->vertSize;
	size_t vertsElements = m_pData->verts.size();
	
	fprintf(m_pFPOut, "\n# %d verticles start\n", vertsElements / vertsSize);

	size_t offset = 0;

	while (offset < vertsElements)
	{
		fprintf(m_pFPOut, "v  ");

		for (size_t i = 0; i < vertsSize; i++)
		{
			if (i == (vertsSize - 1))
				fprintf(m_pFPOut, "%.4f", m_pData->verts[offset + i]);
			else
				fprintf(m_pFPOut, "%.4f ", m_pData->verts[offset + i]);
		}

		fprintf(m_pFPOut, "\n");

		offset += vertsSize;
	}

	fprintf(m_pFPOut, "\n# %d verticles end\n", vertsElements / vertsSize);
}

void ModObjAsynchExporter::ExportNormals() const
{
	size_t normalsElements = m_pData->normals.size();
	size_t normalsCount = normalsElements / 3;

	if (normalsElements == 0)
		return;
	
	fprintf(m_pFPOut, "\n# %d normals start\n", normalsCount);

	size_t offset = 0;

	while (offset < normalsElements)
	{
		const float* f = &m_pData->normals[offset];

		fprintf(m_pFPOut, "vn %.4f %.4f %.4f\n", f[0], f[1], f[2]);
		offset += 3;
	}

	fprintf(m_pFPOut, "\n# %d normals end\n", normalsCount);
}

void ModObjAsynchExporter::ExportUV() const
{
	size_t uvElements = m_pData->uvs.size();
	size_t uvCount = uvElements / m_pData->uvSize;
	size_t uvSize = m_pData->uvSize;

	fprintf(m_pFPOut, "\n# %d UV start\n", uvCount);

	size_t offset = 0;

	while (offset < uvElements)
	{
		const float* f = &m_pData->uvs[offset];

		fprintf(m_pFPOut, "vt  ");

		for (size_t i = 0; i < uvSize; i++)
		{
			if (i == uvSize - 1)
				fprintf(m_pFPOut, "%.4f", f[i]);
			else
				fprintf(m_pFPOut, "%.4f ", f[i]);
		}

		fprintf(m_pFPOut, "\n");


		offset += uvSize;
	}

	fprintf(m_pFPOut, "\n# %d UV end\n", uvCount);
}

void ModObjAsynchExporter::ExportFaces() const
{
	size_t facesElements = m_pData->faces.size();
	size_t facesCount = facesElements / 3;

	fprintf(m_pFPOut, "\n# %d faces start\n", facesCount);

	size_t lastGroup = 0;

	for (size_t i = 0; i < facesElements; i += 3)
	{
		if (m_pData->faces[i].group_id != lastGroup)
		{
			lastGroup = m_pData->faces[i].group_id;
			fprintf(m_pFPOut, "g %s\n", m_pData->groups[lastGroup - 1].name);
		}

		fprintf(m_pFPOut, "f ");

		for (int j = 0; j < 3; j++)
		{
			auto& f = m_pData->faces[i + j];

			if (f.vert)
				fprintf(m_pFPOut, "%d", f.vert);

			if (f.uv)
				fprintf(m_pFPOut, "/%d", f.uv);

			if (f.norm)
			{
				if (!f.uv)
					fprintf(m_pFPOut, "//%d", f.norm);
				else
					fprintf(m_pFPOut, "/%d", f.norm);
			}

			fprintf(m_pFPOut, "  ");
		}

		fprintf(m_pFPOut, " \n");
	}

	fprintf(m_pFPOut, "\n# %d faces end", facesCount);
}

ModObjAsynchExporter::ModObjAsynchExporter(const ModelOBJ* pModel, const char* fileName)
{
	m_pModel = pModel;
	
}

ModObjAsynchExporter::~ModObjAsynchExporter()
{

}

ITaskStepResult* ModObjAsynchExporter::ExecuteStep(LoaderThread* loaderThread)
{
	//throw std::logic_error("The method or operation is not implemented.");
	return nullptr;
}

void ModObjAsynchExporter::OnCompletion()
{
	//throw std::logic_error("The method or operation is not implemented.");
}
