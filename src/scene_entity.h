/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#pragma once
#include "selection_3d.h"
#include "gl_texture.h"
#include "object_props.h"

enum class EntityClasses
{
	None = 0,
	World,
	GenericEntity,
	Light,
};



#define DECLARE_PROPERTY(Type,Name) protected: Type m_##Name;\
									public: void Set##Name(Type val) { m_##Name = val; } \
											const Type Get##Name() { return m_##Name; } \


class SceneEntity: public ISelectableObject
{
	bool m_bDataLoaded = true;
	
	size_t m_nSerialNumber = 0;
	
	std::size_t m_ClassNameHash;
	std::string m_ClassName;

	// TODO: copy-constructor
	// ���������� ���� ����-��������
	typedef std::pair<std::string, std::string> kvData;
	std::unordered_map<std::string, std::string> m_vProperties;

	EntityClasses m_EntityClass;

protected:
	void SetClassName(const char* name);

	
	void LoadPropertiesToPropsEditor(IObjectPropertiesBinding * binder);

public:
	SceneEntity();
	SceneEntity(SceneEntity& other);
	
	virtual void RenderLightshaded(); // � ���������
	virtual void RenderUnshaded(); // ��� ��������
	virtual void RenderBoundingBox(); // 
	virtual void RenderDebug(); // ���������� ���������
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
	
	void CopyProperties(std::unordered_map<std::string, std::string> propsmap);
	std::unordered_map<std::string, std::string> GetProperties() const;

	virtual EntityClasses EntityClass();
	void FlagDataLoaded();
};

typedef std::shared_ptr<SceneEntity> SceneEntityPtr;
typedef std::weak_ptr<SceneEntity>	SceneEntityWeakPtr;

