/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"

#include "bsp_entities_properties_binding.h"
#include "bsp_entity.h"
#include "bsp_entity_property.h"

#include "common.h"
#include "goldsource_game_configuration.h"
#include "properties_editor.h"
#include "text_utils.h"
#include "wad_textures.h"

using namespace GoldSource;

BSPEntity::BSPEntity(Scene *pScene) : SceneEntity(pScene)
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

    auto fgdClassPtr = m_pFGDClass.lock();

    if (fgdClassPtr)
    {
        propDescr = fgdClassPtr->FindProperty(key);
    }

    size_t prophash = BSPEntityProperty::CalcHash(key);

    BSPEntityProperty *existingProp = FindProperty(prophash);

    if (existingProp)
    {
        existingProp->ParseValue(value);
        assert(existingProp->IsInitialized());
    }
    else
    {
        BSPEntityProperty *pProperty = new BSPEntityProperty(this, key, value, propDescr);
        m_lstProperties.push_back(pProperty);

        assert(pProperty->IsInitialized());
    }
}

bool BSPEntity::IsTransparent()
{
    return m_pEditorSprite != nullptr;
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
    for (auto &it : m_lstProperties)
    {
        if (it->Hash() == prophash)
            return it;
    }

    return nullptr;
}

void BSPEntity::PopulateScene()
{
    auto classPtr = m_pFGDClass.lock();

    if (classPtr)
    {
        auto ch = classPtr->ClassName().c_str();

        const BoundingBox &bbox = classPtr->GetBoundingBox();

        if (glm::length2(bbox.Size()) == 0)
            SetBoundingBox(BoundingBox(8));
        else
            SetBoundingBox(bbox);

        // if (!m_bIsSetColor)
        SetRenderColor(classPtr->GetColor());

        auto model = GetModel();

        if (model.expired())
        {
            m_pEditorSprite = classPtr->GetEditorSpite();

            auto &studioModel = classPtr->GetModel();

            if (!studioModel.empty())
            {
                IModelWeakPtr model = ModelsManager::Instance()->LookupModel(studioModel.c_str(), false);
                if (!model.expired())
                {
                    SetModel(model);
                }
            }
        }
        else
        {
            auto ptr         = model.lock();
            auto boundingBox = ptr->GetBoundingBox();

            if (boundingBox.has_value())
            {
                SetBoundingBox(boundingBox.value());                
            }
        }
    }
    else
    {
        SetBoundingBox(BoundingBox(8));

        // if (!m_bIsSetColor)
        SetRenderColor({1, 0, 1, 1});
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
    for (auto &it : m_lstProperties)
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
        BSPEntitiesPropertiesBinder *pBinder = new BSPEntitiesPropertiesBinder();
        pBinder->SelectEntity(weakRef);
        ObjectPropertiesEditor::Instance()->LoadObject(pBinder);
    }
}

void BSPEntity::RenderUnshaded()
{
    const IModelWeakPtr model = GetModel();

    auto sr = Application::GetMainWindow()->GetSceneRenderer();

    if (auto ptr = model.lock())
    {
        ptr->Render(this, RenderMode::Unshaded);
        return;
    }

    const BoundingBox &relativeBbox = GetRelativeBoundingBox();

    if (m_pEditorSprite)
    {
        // TODO: cache this?
        auto prop = FindProperty(SpecialKeys::Key_Light());

        glm::vec3 tint = {1, 1, 1};

        if (prop)
            tint = prop->GetColorRGB();

        const glm::vec3 size = relativeBbox.Size();

        sr->DrawBillboard(GetPosition(), size.xy, m_pEditorSprite, tint, GetSerialNumber());
    }
    else
    {
        sr->RenderPointEntityDefault(GetPosition(), relativeBbox.Mins(), relativeBbox.Maxs(), GetRenderColor(),
                                     GetSerialNumber());
    }
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
    auto color     = pEntity->GetRenderColor() * 255.f;
    auto intensity = pEntity->GetIntensity();

    return glm::vec4(color.r, color.g, color.b, intensity);
}

void BSPEntity::SetFGDClass(FGDEntityClassWeakPtr pClass)
{
    m_pFGDClass = pClass;

    auto classPtr = m_pFGDClass.lock();

    if (!classPtr)
        return;

    for (auto &it : m_lstProperties)
    {
        if (it->PropertyDescriptor())
            continue;

        auto descr = classPtr->FindProperty(it->Name());
        it->SetDescriptor(descr);
    }

    for (auto &fgdProp : classPtr->GetProperties())
    {
        auto hash = BSPEntityProperty::CalcHash(fgdProp->GetName());

        if (HasProperty(hash))
            continue;

        BSPEntityProperty *pProperty = new BSPEntityProperty(this, fgdProp);
        m_lstProperties.push_back(pProperty);
    }
}

GoldSource::FGDEntityClassWeakPtr BSPEntity::GetFGDClass()
{
    return m_pFGDClass;
}
