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
#include "bsp_entities_properties_binding.h"
#include "properties_editor.h"

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

void GoldSource::BSPEntity::SetKeyValue(const std::string &key, const std::string &value)
{   
    FGDPropertyDescriptor *propDescr = nullptr;

    if (m_pFGDClass)
    {
        propDescr = m_pFGDClass->FindProperty(key);
    }

    size_t prophash = BSPEntityProperty::CalcHash(key);

    BSPEntityProperty *existingProp = FindProperty(prophash);

    if (existingProp)
    {
        existingProp->ParseValue(value);
    }
    else
    {
        BSPEntityProperty *pProperty = new BSPEntityProperty(this, key, value, propDescr);
        m_lstProperties.push_back(pProperty);
    }
}

void BSPEntity::UpdateProperty(BSPEntityProperty *pNewProperty)
{
    auto myProperty = FindProperty(pNewProperty->Hash());

    // Shouldn't be nullptr
    assert(myProperty);

    myProperty->Update(pNewProperty);
}

BSPEntityProperty *BSPEntity::FindProperty(size_t prophash)
{
    for(auto & it: m_lstProperties)
    {
        if (it->Hash() == prophash)
            return it;
    }

    return nullptr;
}


void BSPEntity::PopulateScene()
{
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
      
    std::shared_ptr<SceneEntity> ptr(this);
    m_pScene->AddNewSceneEntity(ptr);
}

void BSPEntity::Export(FILE *fp)
{
    fprintf(fp, "{\n");
   
    for (auto &it : m_lstProperties)
    {
        if (it->IsInitialized())
            it->SerializeAsKeyValue(fp);
    }
   
   fprintf(fp, "}\n");
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

void BSPEntity::OnSelect(ISelectableObjectWeakRef myWeakRef)
{    
    auto ptr = myWeakRef.lock();
    
    if (ptr)
    {
        SceneEntityWeakPtr weakRef = std::dynamic_pointer_cast<SceneEntity>(myWeakRef.lock());
        m_pScene->HintSelected(weakRef);

        // TODO: make binder singletons?
        BSPEntitiesPropertiesBinder *pBinder  = new BSPEntitiesPropertiesBinder();
        pBinder->SelectEntity(weakRef);        
        ObjectPropertiesEditor::Instance()->LoadObject(pBinder);
    }
    
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

glm::vec4 BSPEntity::ConvertLightColorAndIntensity(Lb3kLightEntity *pEntity)
{
    auto color     = pEntity->GetColor() * 255.f;
    auto intensity = pEntity->GetIntensity();

    return glm::vec4(color.r, color.g, color.b, intensity);
}

void BSPEntity::SetFGDClass(FGDEntityClass *pClass)
{
    m_pFGDClass = pClass;

    if (!m_pFGDClass)
        return;

    for (auto & it: m_lstProperties)        
    {
        if (it->PropertyDescriptor())
            continue;

        auto descr = m_pFGDClass->FindProperty(it->Name());
        it->SetDescriptor(descr);       
    }

    for (auto &fgdProp : pClass->GetProperties())
    {
        auto hash = BSPEntityProperty::CalcHash(fgdProp->GetName());

        if (HasProperty(hash))
            continue;

        BSPEntityProperty *pProperty = new BSPEntityProperty(this, fgdProp);
        m_lstProperties.push_back(pProperty);

    }

}

GoldSource::FGDEntityClass *BSPEntity::GetFGDClass()
{
    return m_pFGDClass;
}
