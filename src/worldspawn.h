/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/


#pragma once
#include "scene_entity.h"
#include "object_props.h"
#include "model_obj.h"
#include "goldsource_bsp_world.h"
#include "goldsource_bsp_renderer.h"

class Worldspawn: public SceneEntity
{
	ModelOBJ* m_pObjWorld = nullptr;

	GoldSource::BSPWorld* m_pBSPWorld = nullptr;
	GoldSource::BSPRenderer* m_pBSPRenderer = nullptr;

public:
	Worldspawn(const char* fileName);
	~Worldspawn();

	DECLARE_PROPERTY(glm::vec3, EnvColor);
	DECLARE_PROPERTY(std::string, Skybox);

	void OnSelect() override;
	std::string ExportForCompiling(const char* newPath, lightBakerSettings_t* lb3kOptions);

	void RenderBoundingBox() override;
	void RenderDebug() override;
	void RenderForSelection(int objectId, class SceneRenderer*) override;
	void RenderGroupShaded() override;
	void RenderLightshaded() override;
	void RenderUnshaded() override;


	bool IsDataLoaded() override;

	void ReloadLightmaps();
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


};