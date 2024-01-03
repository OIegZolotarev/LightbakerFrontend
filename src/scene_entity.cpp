/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "scene_entity.h"
#include "properties_editor.h"

void SceneEntity::RecalcAbsBBox()
{
    m_EntVars.bboxAbsolute = BoundingBox(m_EntVars.origin, m_EntVars.bboxRelative);
}

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


std::weak_ptr<SceneEntity> SceneEntity::Next()
{
    return m_pNext;
}

void SceneEntity::SetNext(std::weak_ptr<SceneEntity> &pOther)
{

#ifdef PARANOID
    auto ptr = pOther.lock();

    if (ptr.get() == this)
        __debugbreak();
#endif

    m_pNext = pOther;
}

void SceneEntity::SetTransform(glm::mat4 m_matGuizmo)
{
    m_EntVars.transform = m_matGuizmo;
}

void SceneEntity::Debug_RenderTransform()
{
    BT_PROFILE("SceneEntity::Debug_RenderTransform()");

    auto drawAxis = [](glm::vec3 pos, glm::vec3 dir, float size, glm::vec4 color) {
        auto shader = GLBackend::Instance()->SolidColorGeometryShader();
        shader->Bind();

        for (auto &it : shader->Uniforms())
        {
            switch (it->Kind())
            {
            case UniformKind::Color:
                it->SetFloat4(color);
                break;
            case UniformKind::TransformMatrix:
                it->SetMat4(glm::mat4(1));
                break;
            case UniformKind::ObjectSerialNumber:
                it->SetInt(0);
                break;
            default:
                GLBackend::SetUniformValue(it);
                break;
            }
        }

        static DrawMesh mesh(DrawMeshFlags::Dynamic);

        mesh.Begin(GL_LINES);

        auto p2 = (pos + dir * size);

        mesh.Vertex3fv(&pos.x);
        mesh.Vertex3fv(&p2.x);

        mesh.End();
        mesh.BindAndDraw();

        shader->Unbind();
    };

    glm::vec3 forward = m_EntVars.transform[0];
    glm::vec3 right   = m_EntVars.transform[2];
    glm::vec3 up      = m_EntVars.transform[1];

    drawAxis(m_EntVars.origin, forward, 100, {1, 0, 0, 1});
    drawAxis(m_EntVars.origin, right, 100, {0, 1, 0, 1});
    drawAxis(m_EntVars.origin, up, 100, {0, 0, 1, 1});
}

void SceneEntity::FlagRegisteredInScene(bool state)
{
    m_bRegisteredInScene = state;
}

void SceneEntity::SetOctreeNode(OctreeNode *node)
{
    m_pOctreeNode = node;
}

const OctreeNode *SceneEntity::GetOctreeNode() const
{
    return m_pOctreeNode;
}

const glm::vec3 SceneEntity::GetAngles() const
{
    return m_EntVars.angles;
}

void SceneEntity::SetAngles(const glm::vec3 &angles)
{
    m_EntVars.angles = angles;
    m_EntVars.transform = R_RotateForEntity(m_EntVars.origin, m_EntVars.angles);
}

// void SceneEntity::LoadPropertiesToPropsEditor(IObjectPropertiesBinding *binder)
// {
//     auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();
//     auto scene         = sceneRenderer->GetScene();
// 
//     auto weakRef = scene->GetEntityWeakRef(this);
//     scene->HintSelected(weakRef);
// 
//     ObjectPropertiesEditor::Instance()->LoadObject(binder);
// }

SceneEntity::SceneEntity(Scene *pScene)
{
    m_pScene = pScene;

    SetRenderColor({1, 1, 1, 1});
}

SceneEntity::SceneEntity(SceneEntity &other)
{
    m_pScene   = other.m_pScene;
    
    m_EntVars.bboxRelative = other.m_EntVars.bboxRelative;
    m_EntVars.bboxAbsolute = other.m_EntVars.bboxAbsolute;
    
    m_EntVars.rendercolor = other.m_EntVars.rendercolor;
}

bool SceneEntity::IsDataLoaded()
{
    return m_bDataLoaded;
}

void SceneEntity::SetSerialNumber(const uint32_t newNum)
{
    m_EntVars.serialNumber = newNum;
}

const uint32_t SceneEntity::GetSerialNumber() const
{
    return m_EntVars.serialNumber;
}

void SceneEntity::SetPosition(const glm::vec3 &pos)
{
    m_EntVars.origin = pos;
    RecalcAbsBBox();

    m_EntVars.transform = R_RotateForEntity(m_EntVars.origin, m_EntVars.angles);
}

const glm::vec3 SceneEntity::GetPosition() const
{
    return m_EntVars.origin;
}

const glm::mat4 SceneEntity::GetTransform()
{
    return m_EntVars.transform;
}

void SceneEntity::SetBoundingBox(const BoundingBox &bbox)
{
    m_EntVars.bboxRelative = bbox;
    RecalcAbsBBox();
}

void SceneEntity::SetRenderColor(const ColorRGBA &color)
{
    m_EntVars.rendercolor = color;
}

const ColorRGBA SceneEntity::GetRenderColor() const
{
    return m_EntVars.rendercolor;
}

IModelWeakPtr SceneEntity::GetModel() const
{
    return m_EntVars.model;
}

void SceneEntity::SetModel(IModelWeakPtr &model)
{
    m_EntVars.model = model;
}

void SceneEntity::SetFrame(const float newVal)
{
    m_EntVars.frame = newVal;
}

const float SceneEntity::GetFrame() const
{
    return m_EntVars.frame;
}

void SceneEntity::OnHovered()
{
}

void SceneEntity::OnMouseMove(glm::vec2 delta)
{
}

void SceneEntity::OnSelect(ISelectableObjectWeakRef myWeakRef)
{
    ObjectPropertiesEditor::Instance()->UnloadObject();
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

void SceneEntity::OnAdditionToScene(class Scene *pScene)
{
}

EntityClasses SceneEntity::EntityClass()
{
    return EntityClasses::GenericEntity;
}
