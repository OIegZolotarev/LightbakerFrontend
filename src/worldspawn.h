/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/


#pragma once
#include "scene_entity.h"
#include "object_props.h"
#include "model_obj.h"

class Worldspawn: public SceneEntity
{
	ModelOBJ* m_pObjWorld;

public:

	Worldspawn();
	~Worldspawn();

	DECLARE_PROPERTY(glm::vec3, EnvColor);
	DECLARE_PROPERTY(std::string, Skybox);

	void OnSelect() override;
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