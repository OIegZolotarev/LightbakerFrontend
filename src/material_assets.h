/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once
#include "common.h"

class MaterialAssetsProvider;
class MaterialAssetsGroup;
class GameConfiguration;
class GLTexture;

class MaterialAsset
{
    bool       m_bLoaded      = false;
    bool       m_bAnimated    = false;
    GLTexture *m_pEditorImage = nullptr;
    GLTexture *m_pDiffuseMap  = nullptr;

    char m_strAssetName[128];

    const MaterialAssetsGroup *m_pAssetsGroup = nullptr;

public:
    MaterialAsset(const char *m_strAssetName, const MaterialAssetsGroup *pGroup);
    virtual ~MaterialAsset();
};

class MaterialAssetsGroup
{
    std::vector<MaterialAsset *>  m_vecAssets;
    char                          m_strGroupName[128];
    const MaterialAssetsProvider *m_pProvider;

protected:
    void AddAsset(MaterialAsset *item);

    void ReservePool(size_t count);

public:
    MaterialAssetsGroup(const char *groupName, const MaterialAssetsProvider *pProvider);
    virtual ~MaterialAssetsGroup();
};

class MaterialAssetsProvider
{
    const GameConfiguration *          m_pGameConfiguration;
    std::vector<MaterialAssetsGroup *> m_vecGroups;

protected:

    void AddAssetsGroup(MaterialAssetsGroup *pGroup);
    void ReservePool(size_t newSize);

public:
    MaterialAssetsProvider(const GameConfiguration *pGameConfiguration);
    virtual ~MaterialAssetsProvider();
};

typedef std::shared_ptr<MaterialAsset> MaterialAssetPtr;