/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/


#pragma once
#include "scene_entity.h"
#include "object_props.h"
#include "model_obj.h"
#include "goldsource_bsp_level.h"
#include "goldsource_bsp_renderer.h"
#include "world_entity.h"

class ModelObjWorld: public IWorldEntity
{
	ModelOBJ* m_pObjWorld = nullptr;
public:

	ModelObjWorld(const char* fileName);
	~ModelObjWorld();

	DECLARE_PROPERTY(glm::vec3, EnvColor);
	DECLARE_PROPERTY(std::string, Skybox);

	void OnSelect() override;
	std::string ExportForCompiling(const char* newPath, lightBakerSettings_t* lb3kOptions) override;

	void RenderBoundingBox() override;
	void RenderDebug() override;
	void RenderForSelection(int objectId, class SceneRenderer*) override;
	void RenderGroupShaded() override;
	void RenderLightshaded() override;
	void RenderUnshaded() override;


	bool IsDataLoaded() override;

	virtual void ReloadLightmaps();

	void OnAdditionToScene() override;


	EntityClasses EntityClass() override;

};


class WorldspawnPropertiesBinder : public IObjectPropertiesBinding
{
	SceneEntityWeakPtr m_ptr;

	enum PropsId
	{
		EnvColor = 0
	};

public:

	WorldspawnPropertiesBinder(SceneEntityWeakPtr ptr)
	{
		m_ptr = ptr;
	}

	void FillProperties(std::vector<VariantValue>& collection) override;
	bool IsObjectValid() override;
	void OnPropertyChangeSavedToHistory() override;
	void UpdateObjectProperties(VariantValue* props, size_t num) override;



	const char* ObjectClassname() override;

};