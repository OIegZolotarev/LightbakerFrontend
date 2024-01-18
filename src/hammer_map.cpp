/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "hammer_map.h"

extern void ParseMap(HammerMap *file);

HammerMap::HammerMap(FileData *pFileData)
{
    m_pFileData = pFileData;
    m_pFileData->Ref();

    ParseMap(this);
}

HammerMap::~HammerMap()
{
    m_pFileData->UnRef();
}

const std::string & HammerMap::FileName() const
{
    if (!m_pFileData)
    {
        static std::string sEmpty = "";
        return sEmpty;
    }

    return m_pFileData->Name();
}

const char *HammerMap::Data() const
{
    if (!m_pFileData)
        return nullptr;

    return (const char*)m_pFileData->Data();
}

const size_t HammerMap::Length() const
{
    if (!m_pFileData)
        return 0;

    return m_pFileData->Length();
}

void HammerMap::AddEntity(map220_entity_t *entity)
{
    m_lstEntities.push_back(entity);
}
