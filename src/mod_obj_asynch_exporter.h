/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#pragma once

#include "model_obj.h"
#include "loader_thread.h"

class ModObjAsynchExporter : public ITask
{	
	mobjdata_t m_pData;
	ModelOBJ* m_pModel = nullptr;

	std::string m_strFileName;

	FILE* m_pFPOut = nullptr;

	void ExportLightDefs() const;
	void ExportVerticles() const;
	void ExportNormals() const;
	void ExportUV() const;
	void ExportFaces() const;
	void ExportMtlLibs() const;

	bool m_bExportingLightmaps = false;
public:
	ModObjAsynchExporter(ModelOBJ* pModel, const char* fileName, bool exportLMMesh);
	~ModObjAsynchExporter();

	ITaskStepResult* ExecuteStep(LoaderThread* loaderThread) override;
	void OnCompletion() override;

	void ExportSynch();

};

