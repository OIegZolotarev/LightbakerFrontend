/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#pragma once

#include "..\xatlas\source\xatlas\xatlas.h"

class MeshDeclWrapper
{
	xatlas::UvMeshDecl m_MeshDecl;

	glm::vec2* m_pUVData;
	size_t m_nUVDataCount;

	
	size_t m_nFacesCount;
	uint32_t* m_pFaceMaterialData;

	size_t m_GroupId = 0;
	size_t m_XAtlasMeshId = 0;
	
public:
	MeshDeclWrapper(mobjdata_t* modelData, size_t groupId);
	~MeshDeclWrapper();

	xatlas::UvMeshDecl& GetMeshDecl();
	size_t GetGroupId();

	void	SetXAtlasGroupId(size_t meshId);
	size_t	GetXAtlasGroupId();
};

class GenerateAtlasTask : public ITask
{
	std::list<MeshDeclWrapper*> m_lstWrappers;
	ModelOBJ* m_pModel;

	xatlas::Atlas* atlas;
public:
	GenerateAtlasTask(ModelOBJ * pModel);
	~GenerateAtlasTask();

	ITaskStepResult* ExecuteStep(LoaderThread* loaderThread) override;
	void OnCompletion() override;

	void TransferNewUV();


	void DumpUVImage(const char * fileName);
};