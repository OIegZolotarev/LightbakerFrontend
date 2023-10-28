/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once
#include "gl_texture.h"
#include "object_props.h"
#include "selection_3d.h"
#include "scene_entity.h"

// Light flags from LB3k app

#define LF_EULER	(1<<0)
#define LF_DYN		(1<<1)
#define LF_XYZ		(1<<2)
#define LF_LINEAR	(1<<3)
#define LF_DISK		(1<<4)
#define LF_RECT		(1<<5)

enum class LightTypes
{
	Omni = 0,
	Spot,
	Direct
};

// TODO: нужно ли оставить привязки свойств или же сделать этот объект провайдером свойств?
typedef class lightDef_s : public SceneEntity
{
public:
	void UpdateEditorIcon();

	LightTypes	type = LightTypes::Omni;
	int			flags = 0;
	
	float		intensity = 1;

	glm::vec3	anglesDirection;
	float		cones[2];
	glm::vec2	size;

	int			style = 0;

	void OnHovered() override;
	void OnMouseMove(glm::vec2 delta) override;
	void OnUnhovered() override;
	void RenderForSelection(int objectId, class SceneRenderer*) override;
	void OnSelect() override;


	void OnUnSelect() override;

	const char* Description() override;

	int serial_number = 0;

}lightDef_t;

enum LightProperties
{
	Type = 0,
	Flags,
	Pos,
	Color,
	Intensity,
	Angles,
	ConeA,
	ConeB,
	Size,
	Style
};

typedef std::shared_ptr<lightDef_t> lightDefPtr_t;
typedef std::weak_ptr<lightDef_t>	lightDefWPtr_t;

class LightPropertiesBinding : public IObjectPropertiesBinding
{
	lightDefWPtr_t m_pwLightDef;

	void MakeSizeProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr);
	void MakeConeBProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr);
	void MakeConeAProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr);
	void MakeLightIntensityProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr);
	void MakeLightColorProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr);
	void MakeLightAnglesProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr);
	void MakeLightPosProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr);
	void MakeLightFlagsProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr);
	void MakeLightTypeProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr);
	void MakeLightStyleProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr);

public:
	LightPropertiesBinding(lightDefWPtr_t ref);

	void FillProperties(std::vector<propsData_t>& collection) override;
	void UpdateObjectProperties(propsData_t* data, size_t num) override;
	bool IsObjectValid() override;

	int GetSerialNumber() override;
	void OnPropertyChangeSavedToHistory() override;

	ImGuizmo::OPERATION GetMeaningfulGizmoOperationMode() override;

};

