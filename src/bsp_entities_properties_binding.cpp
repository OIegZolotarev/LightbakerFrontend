/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "bsp_entities_properties_binding.h"
#include "goldsource_bsp_entity.h"

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
    throw std::logic_error("The method or operation is not implemented.");
}
