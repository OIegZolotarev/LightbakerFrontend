/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "goldsource_bsp_entity.h"

using namespace GoldSource;

BSPEntity::BSPEntity()
{

}

BSPEntity::~BSPEntity()
{

}

void BSPEntity::SetKeyValue(std::string& key, std::string& value)
{
    m_vProperties.insert(kvData(key, value));
}
