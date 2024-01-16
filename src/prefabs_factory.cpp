/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "prefabs_factory.h"

 IPrefabDepartament::IPrefabDepartament(const char *name)
{
    m_strName = name;
}

IPrefabDepartament::~IPrefabDepartament()
{
}

PrefabsFactory::PrefabsFactory()
{
}

PrefabsFactory::~PrefabsFactory()
{
}

const IPrefabDepartament *PrefabsFactory::GetPrefabDepartament(size_t idx) const
{
    return m_lstDepartaments[idx];
}
