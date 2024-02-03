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

MaterialAssetsGroup::MaterialAssetsGroup(const char *groupName, const MaterialAssetsProvider *pProvider)
{
     m_pProvider = pProvider;
    strlcpy(m_strGroupName, groupName, sizeof(m_strGroupName));
}

MaterialAssetsGroup::~MaterialAssetsGroup()
{
    ClearPointersVector(m_vecAssets);
}

MaterialAsset::MaterialAsset(const char *m_strAssetName, const MaterialAssetsGroup *pGroup)
{
    m_pAssetsGroup = pGroup;
}

MaterialAsset::~MaterialAsset()
{
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

