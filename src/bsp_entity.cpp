/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "bsp_entity.h"
#include "common.h"
#include "goldsource_game_configuration.h"
#include "text_utils.h"
#include "wad_textures.h"
#include "bsp_entity_property.h"

using namespace GoldSource;

BSPEntity::BSPEntity(Scene *pScene): SceneEntity(pScene)
{
}

BSPEntity::~BSPEntity()
{
    for (auto &it : m_lstProperties)
        delete it;

    m_lstProperties.clear();
}

void BSPEntity::SetKeyValue(std::string &key, std::string &value)
{   
    FGDPropertyDescriptor *propDescr = nullptr;

    if (m_pFGDClass)
    {
        propDescr = m_pFGDClass->FindProperty(key);
    }

    BSPEntityProperty *pProperty = new BSPEntityProperty(this, key, value, propDescr);

    m_lstProperties.push_back(pProperty);

    m_vProperties.insert(kvData(key, value));
}

void BSPEntity::PopulateScene()
{
    auto scene      = Application::GetMainWindow()->GetSceneRenderer()->GetScene();
    auto &classname = m_vProperties["classname"];

    GameConfigurationWeakPtr pConfigWeakPtr = scene->UsedGameConfiguration();
    auto pConfigPtr                         = pConfigWeakPtr.lock();
    GameConfiguration *pConfig              = pConfigPtr.get();

    // TODO: do proper type reflection
    {
        GoldSource::HammerGameConfiguration *hammerConfig = (HammerGameConfiguration *)pConfig;

        assert(hammerConfig);

        m_pFGDClass = hammerConfig->LookupFGDClass(classname);
    }

    if (m_pFGDClass)
    {
        SetMins(m_pFGDClass->GetMins());
        SetMaxs(m_pFGDClass->GetMaxs());
        
        if (!m_bIsSetColor)
            SetColor(m_pFGDClass->GetColor());

        m_pEditorSprite = m_pFGDClass->GetEditorSpite();
    }
    else
    {
        SetMins({-4, -4, -4});
        SetMaxs({4, 4, 4});
        
        if (!m_bIsSetColor) 
            SetColor({1, 0, 1});
    }

    SetClassName(classname.c_str());

    std::shared_ptr<SceneEntity> ptr(this);
    scene->AddNewSceneEntity(ptr);
}

bool BSPEntity::UpdateProperties()
{
    //     //auto ptr = m_SceneEntity.lock();
    //
    //     if (!ptr)
    //         return false;
    //
    //     m_vProperties = ptr->GetProperties();
    //
    // 	// ��������� ����� �� �������
    // 	//newOrigin.x = -origin.y;
    // 	//newOrigin.y = origin.z;
    // 	//newOrigin.z = -origin.x;
    //
    //     auto origin = ptr->GetPosition();
    //
    //     std::string newOrigin = std::format("{0} {1} {2}", -origin.z, -origin.x, origin.y);
    //
    //     m_vProperties["origin"] = newOrigin;
    //
    //     if (ptr->IsLightEntity())
    //     {
    //         Lb3kLightEntity* pLight = (Lb3kLightEntity*)ptr.get();
    //         auto _light = ConvertLightColorAndIntensity(pLight);
    //
    //
    //         m_vProperties["_light"] = std::format("{0} {1} {2} {3}", _light.r, _light.g, _light.b, _light.a);
    //
    //     }
    //
    //     return true;

    return true;
}

void BSPEntity::Export(FILE *fp)
{
    fprintf(fp, "{\n");

    for (auto kv : m_vProperties)
    {
        auto &key  = kv.first;
        auto value = kv.second;

        // TODO: ��������� �������������
        // TextUtils::ReplaceAll(value, "\"", "\\"");

        fprintf(fp, "\"%s\" \"%s\"\n", key.c_str(), value.c_str());
    }

    fprintf(fp, "}\n");
}

glm::vec3 BSPEntity::ConvertOriginToSceneSpace(glm::vec3 bspSpaceOrigin)
{
    auto newOrigin = bspSpaceOrigin;

    // ��������� ����� �� �������
    newOrigin.x = -bspSpaceOrigin.y;
    newOrigin.y = bspSpaceOrigin.z;
    newOrigin.z = -bspSpaceOrigin.x;

    return newOrigin;
}

std::list<BSPEntityProperty *> &BSPEntity::GetBSPProperties()
{
    return m_lstProperties;
}

bool BSPEntity::HasProperty(size_t hash)
{
    for (auto & it: m_lstProperties)
    {
        if (it->Hash() == hash)
            return true;
    }

    return false;
}

void BSPEntity::OnSelect()
{
    auto &classname = m_vProperties["classname"];
    Con_Printf("BSPEntity::OnSelect(): %s\n", classname.c_str());
}

void BSPEntity::RenderUnshaded()
{
    auto sr = Application::GetMainWindow()->GetSceneRenderer();

    if (m_pEditorSprite)
        sr->DrawBillboard(m_Position, (m_Maxs - m_Mins).xy, m_pEditorSprite, {1.f, 1.f, 1.f}, GetSerialNumber());
    else
        sr->RenderPointEntityDefault(m_Position, m_Mins, m_Maxs, m_Color, GetSerialNumber());
}

void BSPEntity::RenderLightshaded()
{
    RenderUnshaded();
}

void BSPEntity::RenderGroupShaded()
{
    RenderUnshaded();
}

void BSPEntity::RenderBoundingBox()
{
}

glm::vec3 BSPEntity::ConvertOriginFromSceneSpace(glm::vec3 origin)
{
    return glm::vec3{-origin.z, -origin.x, origin.y};
}

glm::vec4 BSPEntity::ConvertLightColorAndIntensity(Lb3kLightEntity *pEntity)
{
    auto color     = pEntity->GetColor() * 255.f;
    auto intensity = pEntity->GetIntensity();

    return glm::vec4(color.r, color.g, color.b, intensity);
}

void BSPEntity::SetFGDClass(FGDEntityClass *pClass)
{
    m_pFGDClass = pClass;
}
