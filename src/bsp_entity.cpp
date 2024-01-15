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
#include "mod_primitive.h"
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

void BSPEntity::SetKeyValue(const std::string &key, const std::string &value)
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

BSPEntity::BSPEntity(BSPEntity *pOther) : SceneEntity(pOther)
{
    m_pFGDClass = pOther->m_pFGDClass;
    m_World     = pOther->m_World;

    for (auto &it : pOther->m_lstProperties)
    {
        BSPEntityProperty *pNewProp = new BSPEntityProperty(it, this);
        m_lstProperties.push_back(pNewProp);
    }

    m_bIsTransparent = pOther->m_bIsTransparent;
}

void GoldSource::BSPEntity::Render(RenderMode mode, const SceneRenderer *sr, ShaderProgram *shader)
{
    const IModelWeakPtr model = GetModel();

    // TODO: move this somewhere to BSPEntityProperty::Update
    auto prop = FindProperty(SpecialKeys::Key_Light());
    if (prop)
        SetRenderColor(prop->GetColorRGBA());

    auto ptr = model.lock();

    assert(ptr && "Should always have a model for an entity");
    ptr->Render(this, sr, mode, shader);
}

SceneEntity *BSPEntity::Clone()
{
    BSPEntity *pClone = new BSPEntity(this);
    return pClone;
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

        SetRenderColor(classPtr->GetColor());

        auto model = GetModel();

        if (model.expired())
        {
            auto  editorSprite = classPtr->GetEditorSpite();
            auto &studioModel  = classPtr->GetModelName();

            if (!studioModel.empty())
            {
                IModelWeakPtr model = ModelsManager::Instance()->LookupModel(studioModel.c_str(), false);
                if (!model.expired())
                    SetModel(model);
                else
                {
                    auto model = PrimitiveModel::LookupByType(CommonPrimitives::Box);
                    SetModel(model);
                }
            }
            else if (!editorSprite.expired())
            {
                SetModel(editorSprite);
                SetRenderColor({1, 1, 1, 1});
            }
            else
            {
                auto model = PrimitiveModel::LookupByType(CommonPrimitives::Box);
                SetModel(model);
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
        SetRenderColor({1, 0, 1, 1});

        auto model = PrimitiveModel::LookupByType(CommonPrimitives::Box);
        SetModel(model);
    }

    assert(!GetModel().expired());

    m_pScene->AddNewSceneEntity(this);
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
        // m_pScene->HintSelected(weakRef);

        auto bindings = ObjectPropertiesEditor::Instance()->GetBindings();

        if (bindings)
        {
            // ObjectPropertiesEditor::Instance()->LoadObject(weakRef, m_);
            // bindings->AddObject(weakRef);
        }
        else
        {
            // TODO: make binder singletons?
            //             BSPEntitiesPropertiesBinder *pBinder = new BSPEntitiesPropertiesBinder();
            //             pBinder->SelectEntity(weakRef);
            //            ObjectPropertiesEditor::Instance()->LoadObject(pBinder);
        }
    }
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

void BSPEntity::UpdatePropertyPosition(BSPEntityProperty *pNewProperty, glm::vec3 delta)
{
    auto myProperty = FindProperty(pNewProperty->Hash());

    // Shouldn't be nullptr
    assert(myProperty);

    myProperty->SetPosition(myProperty->GetPosition() + delta);
    myProperty->Update(nullptr);
}
