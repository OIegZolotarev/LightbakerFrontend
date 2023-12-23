/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "scene_entity.h"
#include "properties_editor.h"

const BoundingBox &SceneEntity::GetRelativeBoundingBox() const
{
    return m_EntVars.bboxRelative;
}

const BoundingBox &SceneEntity::GetAbsoulteBoundingBox() const
{
    return m_EntVars.bboxAbsolute;
}

void SceneEntity::SetClassName(const char *name)
{
    m_EntVars.classname = std::string(name);
    m_EntVars.classname_hash = std::hash<const char *>{}(name);
}

void SceneEntity::FlagDataLoaded()
{
    m_bDataLoaded = true;
}

void SceneEntity::InvokeSelect()
{
    auto weakRef = m_pScene->GetEntityWeakRef(this);
    m_bSelected  = true;

    OnSelect(weakRef);
    SelectionManager::Instance()->UnSelectEverythingBut(this);
}

const std::string &SceneEntity::GetClassName() const
{
    return m_EntVars.classname;
}

bool SceneEntity::IsTransparent()
{
    return false;
}

SceneEntityWeakPtr SceneEntity::Next()
{
    return m_pNext;
}

void SceneEntity::SetNext(std::weak_ptr<SceneEntity> &pOther)
{
    auto ptr = pOther.lock();

#ifdef PARANOID
    if (ptr.get() == this)
        __debugbreak();
#endif

    m_pNext = pOther;
}

const BoundingBox &SceneEntity::AbsoulteBoundingBox() const
{
    return m_EntVars.bboxAbsolute;
}

void SceneEntity::LoadPropertiesToPropsEditor(IObjectPropertiesBinding *binder)
{
    auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();
    auto scene         = sceneRenderer->GetScene();

    auto weakRef = scene->GetEntityWeakRef(this);
    scene->HintSelected(weakRef);

    ObjectPropertiesEditor::Instance()->LoadObject(binder);
}

SceneEntity::SceneEntity(Scene *pScene)
{
    m_pScene = pScene;
    

    // m_Color      = {0, 0, 0};
    // m_EditorIcon = nullptr;

    SetEditorIcon(nullptr);
    SetRenderColor({1, 1, 1, 1});
}

SceneEntity::SceneEntity(SceneEntity &other)
{
    m_pScene   = other.m_pScene;
    
    m_EntVars.bboxRelative = other.m_EntVars.bboxRelative;
    m_EntVars.bboxAbsolute = other.m_EntVars.bboxAbsolute;
    
    m_EntVars.rendercolor = other.m_EntVars.rendercolor;
    m_EntVars.editor_icon = other.m_EntVars.editor_icon;

}

void SceneEntity::RenderLightshaded()
{
    auto sceneRenderer = Application::GetMainWindow()->GetSceneRenderer();
    sceneRenderer->RenderGenericEntity(this);
}

void SceneEntity::RenderUnshaded()
{
}

void SceneEntity::RenderBoundingBox()
{
}

void SceneEntity::RenderDebug()
{
}

void SceneEntity::RenderGroupShaded()
{
}

bool SceneEntity::IsDataLoaded()
{
    return m_bDataLoaded;
}

void SceneEntity::OnHovered()
{
}

void SceneEntity::OnMouseMove(glm::vec2 delta)
{
}

void SceneEntity::OnSelect(ISelectableObjectWeakRef myWeakRef)
{
}

void SceneEntity::OnUnSelect()
{
}

void SceneEntity::OnUnhovered()
{
}

const char *SceneEntity::Description()
{
    return m_EntVars.classname.c_str();
}

bool SceneEntity::IsLightEntity()
{
    return false;
}

EntityClasses SceneEntity::EntityClass()
{
    return EntityClasses::GenericEntity;
}
