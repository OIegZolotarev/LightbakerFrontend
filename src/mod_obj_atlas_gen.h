/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#pragma once

#include "..\xatlas\source\xatlas\xatlas.h"

void TestAtlasGeneration(ModelOBJ* pModel);

class MeshDeclWrapper
{
	xatlas::UvMeshDecl m_MeshDecl;

	glm::vec2* m_pUVData;
	size_t m_nUVDataCount;

	
	size_t m_nFacesCount;
	uint32_t* m_pFaceMaterialData;
	
	
public:
	MeshDeclWrapper(mobjdata_t* modelData, size_t groupId);
	~MeshDeclWrapper();

	xatlas::UvMeshDecl& GetMeshDecl()
	{
		return m_MeshDecl;
	}
};


class GenerateAtlasTask : public ITask
{
private:
	MeshDeclWrapper** m_pWrappers;
public:
	GenerateAtlasTask(ModelOBJ * pModel);
	~GenerateAtlasTask();

	ITaskStepResult* ExecuteStep(LoaderThread* loaderThread) override;
	void OnCompletion() override;
};