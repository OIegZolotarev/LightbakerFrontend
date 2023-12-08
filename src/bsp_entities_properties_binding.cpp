/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "bsp_entities_properties_binding.h"
#include "bsp_entity.h"
#include "bsp_entity_property.h"

using namespace GoldSource;

void BSPEntitiesPropertiesBinder::SelectEntity(SceneEntityWeakPtr ptr)
{
    auto lck = ptr.lock();
    
    if (!lck)
        return;

    auto raw = lck.get();

    if (! instanceof <GoldSource::BSPEntity>(raw))
        return;
        
    m_lstSelectedObjects.push_back(ptr);

    RebuildPropertiesList();
}

void BSPEntitiesPropertiesBinder::FillProperties(std::vector<VariantValue> &collection)
{
    
}

const char *BSPEntitiesPropertiesBinder::ObjectClassname()
{
    return "";
}

void BSPEntitiesPropertiesBinder::OnPropertyChangeSavedToHistory()
{
    
}

void BSPEntitiesPropertiesBinder::UpdateObjectProperties(VariantValue *props, size_t num)
{
    
}

void BSPEntitiesPropertiesBinder::RebuildPropertiesList()
{   
    CleanupPropertiesList();
    CleanupDeadObjects();
    
    if (m_lstSelectedObjects.size() == 0)
        return;

    auto ptr = m_lstSelectedObjects.begin();
    auto smart_ptr = ptr->lock();
    GoldSource::BSPEntity *rawPtr    = (GoldSource::BSPEntity *)smart_ptr.get();


    for (auto &it : rawPtr->GetBSPProperties())
    {
        bool addProperty = true;

        if (m_lstSelectedObjects.size() > 1)
        {

            for (auto & otherEntWeak : m_lstSelectedObjects)
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
    for (auto & it: m_lstCommonProperties)
    {
        delete it;
    }

    m_lstCommonProperties.clear();
}
