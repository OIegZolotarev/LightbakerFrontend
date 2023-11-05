/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#pragma once
#include "selection_3d.h"
#include "gl_texture.h"
#include "object_props.h"

enum class EntityClass
{
	None = 0,
	Lights,
};



#define DECLARE_PROPERTY(Type,Name) private: Type m_##Name;\
									public: void Set##Name(Type val) { m_##Name = val; } \
											const Type Get##Name() { return m_##Name; } \


class SceneEntity: public ISelectableObject
{
	bool m_bDataLoaded = false;
	
	size_t m_nSerialNumber = 0;
	
	std::size_t m_ClassNameHash;
	std::string m_ClassName;

protected:
	void SetClassName(const char* name);

	void FlagDataLoaded();
	void LoadPropertiesToPropsEditor(IObjectPropertiesBinding * binder);

public:
	SceneEntity();
	
	virtual void RenderLightshaded(); // С лайтмапой
	virtual void RenderUnshaded(); // Без лайтмапы
	virtual void RenderBoundingBox(); // 
	virtual void RenderDebug(); // Отладочная отрисовка
	virtual void RenderGroupShaded();

	virtual bool IsDataLoaded();
	
	DECLARE_PROPERTY(size_t, SerialNumber);
	DECLARE_PROPERTY(glm::vec3, Position);

	DECLARE_PROPERTY(glm::vec3, Mins);
	DECLARE_PROPERTY(glm::vec3, Maxs);

	DECLARE_PROPERTY(glm::vec3, Color);	
	DECLARE_PROPERTY(gltexture_t*, EditorIcon);

	void OnHovered() override;
	void OnMouseMove(glm::vec2 delta) override;
	void OnSelect() override;
	void OnUnSelect() override;
	void OnUnhovered() override;
	void RenderForSelection(int objectId, class SceneRenderer*) override;

	virtual const char* Description();
	virtual bool IsLightEntity();
	virtual void OnAdditionToScene() {};
	
};

typedef std::shared_ptr<SceneEntity> SceneEntityPtr;
typedef std::weak_ptr<SceneEntity>	SceneEntityWeakPtr;

