/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "material_assets.h"
#include "text_utils.h"


void MaterialAssetsGroup::AddAsset(MaterialAsset *item)
{
    m_vecAssets.push_back(item);
}

void MaterialAssetsGroup::ReservePool(size_t count)
{
    m_vecAssets.reserve(count);
}

void MaterialAssetsGroup::SetGroupName(const char *name)
{
    strlcpy(m_strGroupName, name, sizeof(m_strGroupName));
}

MaterialAssetsGroup::MaterialAssetsGroup(const char *groupName, const MaterialAssetsProvider *pProvider)
{
     m_pProvider = pProvider;
     
     if (groupName)
        strlcpy(m_strGroupName, groupName, sizeof(m_strGroupName));
}

MaterialAssetsGroup::~MaterialAssetsGroup()
{
    ClearPointersVector(m_vecAssets);
}

const char *MaterialAssetsGroup::Description() const
{
    return m_strGroupName;
}

MaterialAsset::MaterialAsset(const char *strAssetName, const MaterialAssetsGroup *pGroup)
{
    m_pAssetsGroup = pGroup;
    strlcpy(m_strAssetName, strAssetName, sizeof(strAssetName));
}

MaterialAsset::~MaterialAsset()
{
}

const char *MaterialAsset::Description() const
{
    return m_strAssetName;
}

void MaterialAssetsProvider::AddAssetsGroup(MaterialAssetsGroup *pGroup)
{
    m_vecGroups.push_back(pGroup);
}

void MaterialAssetsProvider::ReservePool(size_t newSize)
{
    m_vecGroups.reserve(newSize);
}

MaterialAssetsProvider::MaterialAssetsProvider(const GameConfiguration *pGameConfiguration)
{
    m_pGameConfiguration = pGameConfiguration;
}

MaterialAssetsProvider::~MaterialAssetsProvider()
{
    ClearPointersVector(m_vecGroups);
}

