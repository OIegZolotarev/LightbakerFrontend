/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "bsp_entities_properties_binding.h"
#include "bsp_entity.h"
#include "bsp_entity_property.h"
#include "properties_editor.h"

using namespace GoldSource;

void BSPEntitiesPropertiesBinder::SelectEntity(SceneEntityWeakPtr ptr)
{
    BSPEntity *rawBSP = SceneEntity::GetRawSafest<BSPEntity>(ptr);

    if (!rawBSP)
        return;

    // Dont't add if already selected
    for (auto &it : m_lstSelectedObjects)
    {
        auto ptr = it.lock();
        if (ptr.get() == rawBSP)
            return;
    }

    // Con_Printf("SelectEntity(): %s\n", rawBSP->GetClassName().c_str());

    rawBSP->SetSelected(true);

    m_lstSelectedObjects.push_back(ptr);

    RebuildPropertiesList();
    BuildSelectionRepresentation();
}

void BSPEntitiesPropertiesBinder::BuildSelectionRepresentation()
{
    bool multipleClasses = false;

    if (m_lstSelectedObjects.size() == 0)
        return;

    auto       it     = m_lstSelectedObjects.begin();
    BSPEntity *rawBSP = SceneEntity::GetRawSafest<BSPEntity>(*it);

    auto &firstClass = rawBSP->GetClassName();

    assert(rawBSP);

    if (m_lstSelectedObjects.size() > 1)
    {
        for (auto &it : m_lstSelectedObjects)
        {
            auto ptr = it.lock();

            if (!ptr)
                continue;

            if (ptr->GetClassName() != firstClass)
            {
                multipleClasses = true;
                break;
            }
        }
    }

    if (multipleClasses)
    {
        m_strObjectsClassname = std::format("{0} objects", m_lstSelectedObjects.size());
        m_pSelectedClass      = FGDEntityClassWeakPtr();
    }
    else
    {
        m_pSelectedClass = rawBSP->GetFGDClass();

        if (m_lstSelectedObjects.size() == 1)
            m_strObjectsClassname = std::format("{0}", firstClass);
        else
            m_strObjectsClassname = std::format("{1} {0}s", firstClass, m_lstSelectedObjects.size());
    }
}

void GoldSource::BSPEntitiesPropertiesBinder::FillProperties(std::list<VariantValue *> &collection)
{
    for (auto &it : m_lstCommonProperties)
    {
        collection.push_back(it);
    }
}

void GoldSource::BSPEntitiesPropertiesBinder::OnSelectionResized(
    std::unordered_map<uint32_t, glm::vec3> &relativePositions, glm::vec3 scale, glm::vec3 centerPos)
{
    for (auto &it : m_lstSelectedObjects)
    {
        BSPEntity *rawBsp = SceneEntity::GetRawSafest<BSPEntity>(it);

        if (!rawBsp)
            continue;

        assert(relativePositions.contains(rawBsp->GetSerialNumber()));

        glm::vec3 newPos = relativePositions.at(rawBsp->GetSerialNumber()) * scale;

        auto bspProperty = rawBsp->FindProperty(SpecialKeys::KeyOrigin());
        bspProperty->SetPosition(newPos + centerPos);
        bspProperty->Update(nullptr);
    }
}

void BSPEntitiesPropertiesBinder::FinishBoxSelection()
{
    RebuildPropertiesList();
    BuildSelectionRepresentation();
}

void BSPEntitiesPropertiesBinder::AddObjectBoxSelection(SceneEntityWeakPtr &pObject)
{
    auto rawBSP = SceneEntity::GetRawSafest<BSPEntity>(pObject);

    if (!rawBSP)
        return;

    // Dont't add if already selected
    for (auto &it : m_lstSelectedObjects)
    {
        auto ptr = it.lock();
        if (ptr.get() == rawBSP)
            return;
    }

    rawBSP->SetSelected(true);
    m_lstSelectedObjects.push_back(pObject);
}

// void BSPEntitiesPropertiesBinder::OnSelectionResized(BoundingBox m_AllObjectsBounds, BoundingBox rel)
// {
//     glm::vec3 oldSize = m_AllObjectsBounds.Size();
//     glm::vec3 oldMins = m_AllObjectsBounds.Mins();
//
//     glm::vec3 newMins = rel.Mins();
//     glm::vec3 newSize = rel.Size();
//
//     for (auto & it: m_lstSelectedObjects)
//     {
//         BSPEntity *rawBsp = SceneEntity::GetRawSafest<BSPEntity>(it);
//
//         if (!rawBsp)
//             continue;
//
//         // Position in range between [0-1,0-1,0-1]
//         glm::vec3 pos = (rawBsp->GetPosition() - oldMins) / oldSize;
//
//         auto bspProperty = rawBsp->FindProperty(SpecialKeys::KeyOrigin());
//
//         if (!bspProperty)
//             continue;
//
//         auto newPos = newMins + (pos * newSize);
//
//         Con_Printf("Moving to %f %f %f\n", newPos.x, newPos.y, newPos.z);
//
//
//         bspProperty->SetPosition(newPos);
//         bspProperty->Update(nullptr);
//     }
//
// }

void BSPEntitiesPropertiesBinder::UpdatePropertyPositionDelta(VariantValue *propertyPosition, glm::vec3 delta)
{
    if (! instanceof <BSPEntityProperty>(propertyPosition))
        return;

    BSPEntityProperty *p = (BSPEntityProperty *)propertyPosition;

    for (auto &it : m_lstSelectedObjects)
    {
        BSPEntity *rawBsp = SceneEntity::GetRawSafest<BSPEntity>(it);

        if (!rawBsp)
            continue;

        rawBsp->UpdatePropertyPosition(p, delta);
    }
}

void BSPEntitiesPropertiesBinder::ClearObjects()
{
    for (auto &it : m_lstSelectedObjects)
    {
        auto ptr = it.lock();

        if (!ptr)
            continue;

        ptr->SetSelected(false);
    }

    m_lstSelectedObjects.clear();
}

void BSPEntitiesPropertiesBinder::AddObject(SceneEntityWeakPtr weakRef)
{
    SelectEntity(weakRef);
}

SceneEntity *BSPEntitiesPropertiesBinder::GetEntity(int param1)
{
    if ((m_lstSelectedObjects.size()) <= param1)
        return nullptr;

    auto it = m_lstSelectedObjects.begin();
    std::advance(it, param1);

    auto ptr = it->lock();

    if (!ptr)
        return nullptr;

    return ptr.get();
}

void BSPEntitiesPropertiesBinder::UpdateProperty(VariantValue *prop)
{
    if (! instanceof <BSPEntityProperty>(prop))
        return;

    BSPEntityProperty *p = (BSPEntityProperty *)prop;

    for (auto &it : m_lstSelectedObjects)
    {
        auto rawBsp = SceneEntity::GetRawSafest<BSPEntity>(it);

        if (!rawBsp)
            continue;

        rawBsp->UpdateProperty(p);
    }
}

ImGuizmo::OPERATION BSPEntitiesPropertiesBinder::GetMeaningfulGizmoOperationMode()
{
    return ImGuizmo::TRANSLATE;
    // ImGuizmo::TRANSLATE | (ImGuizmo::ROTATE_X | ImGuizmo::ROTATE_Y | ImGuizmo::ROTATE_Z);
}

void BSPEntitiesPropertiesBinder::RenderFooter()
{
    auto classPtr = m_pSelectedClass.lock();

    if (!classPtr)
        return;

    auto &descr = classPtr->Description();

    if (!descr.empty())
        ImGui::TextWrapped("%s - %s", classPtr->ClassName().c_str(), descr.c_str());
}

const char *BSPEntitiesPropertiesBinder::ObjectClassname()
{
    return m_strObjectsClassname.c_str();
}

void BSPEntitiesPropertiesBinder::OnPropertyChangeSavedToHistory()
{
}

void BSPEntitiesPropertiesBinder::RebuildPropertiesList()
{
    CleanupPropertiesList();
    CleanupDeadObjects();

    if (m_lstSelectedObjects.size() == 0)
        return;

    auto                   ptr       = m_lstSelectedObjects.begin();
    auto                   smart_ptr = ptr->lock();
    GoldSource::BSPEntity *rawPtr    = (GoldSource::BSPEntity *)smart_ptr.get();

    for (auto &it : rawPtr->GetBSPProperties())
    {
        bool addProperty = true;

        if (m_lstSelectedObjects.size() > 1)
        {
            for (auto &otherEntWeak : m_lstSelectedObjects)
            {
                // Fine to check against self
                auto otherEnt = otherEntWeak.lock();

                // Should not triggered
                //
                assert(otherEnt);

                GoldSource::BSPEntity *rawPtrOther = (GoldSource::BSPEntity *)otherEnt.get();

                if (!rawPtrOther->HasProperty(it->Hash()))
                {
                    addProperty = false;
                    break;
                }
            }
        }

        if (addProperty)
        {
            m_lstCommonProperties.push_back(new GoldSource::BSPEntityProperty(it));
        }
    }

    ObjectPropertiesEditor::Instance()->ReloadPropertiesFromBinder();
}

void BSPEntitiesPropertiesBinder::CleanupDeadObjects()
{
    for (auto it = m_lstSelectedObjects.begin(); it != m_lstSelectedObjects.end(); it++)
    {
        auto ptr = (*it).lock();

        if (!ptr)
        {
            m_lstSelectedObjects.erase(it++);

            if (it == m_lstSelectedObjects.end())
                break;
        }
    }
}

void BSPEntitiesPropertiesBinder::CleanupPropertiesList()
{
    for (auto &it : m_lstCommonProperties)
    {
        delete it;
    }

    m_lstCommonProperties.clear();
}
