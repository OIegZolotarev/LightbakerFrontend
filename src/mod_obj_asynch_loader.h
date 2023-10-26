/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#pragma once

#include "loader_thread.h"
#include "mod_obj_data.h"



class ModObjAsynchLoader : public ITask
{
	enum class LightParsingStage
	{
		NotEncountered = 0,
		Parsing,
		DoneParsing
	};

	enum class StateMachineResource
	{
		Object,
		Group,
		Material
	};

	std::string m_strFileName;
	FileData* m_pFileData = nullptr;
	size_t m_FileOffset = 0;
	
	ModelOBJ* m_pModel = nullptr;
	mobjdata_t* m_Data = nullptr;

	LightParsingStage m_LightsParsingStage = LightParsingStage::NotEncountered;

	void ParseVertex(std::string& s);
	void ParseUV(std::string& s);
	void ParseNormal(std::string& s);
	void ParseLightDef(std::string& buffer);
	void ParseFace(std::string& s);
	void ParseGroup(std::string& buffer);
	void ParseCommand(std::string& buffer);
	void ParseUseMtl(std::string& buffer);
	void ParseObject(std::string& buffer);
	void ParseMaterialLib(std::string& buffer);

	size_t CurrentResource(StateMachineResource id);

public:
	
	ModObjAsynchLoader(ModelOBJ * pModel, const char* fileName);
	~ModObjAsynchLoader();
	
	ITaskStepResult* ExecuteStep(LoaderThread* loaderThread) override;
	void OnCompletion() override;


	class BuildDrawMeshTask: public ITaskStepResult
	{
	public:
		BuildDrawMeshTask(mobjdata_t * data, ModelOBJ * pModel);
		void ExecuteOnCompletion();
	};
};



