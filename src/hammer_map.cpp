/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "hammer_map.h"

HammerMap::HammerMap(FileData *pFileData)
{
    m_pFileData = pFileData;
    m_pFileData->Ref();
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
