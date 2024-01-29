/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "material_assets.h"
#include "text_utils.h"


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

MaterialAssetsProvider::MaterialAssetsProvider(const GameConfiguration *pGameConfiguration)
{
    m_pGameConfiguration = pGameConfiguration;
}

MaterialAssetsProvider::~MaterialAssetsProvider()
{
    ClearPointersVector(m_vecProviders);
}

