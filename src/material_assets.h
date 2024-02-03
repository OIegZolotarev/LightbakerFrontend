/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once
#include "common.h"
#include "user_presentable_object.h"

class MaterialAssetsProvider;
class MaterialAssetsGroup;
class GameConfiguration;
class GLTexture;

class MaterialAsset : public IUserPresentableObject
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

    const char *Description() const override;
};

class MaterialAssetsGroup : public IUserPresentableObject
{
    std::vector<MaterialAsset *>  m_vecAssets;
    char                          m_strGroupName[128];
    const MaterialAssetsProvider *m_pProvider;

protected:
    void AddAsset(MaterialAsset *item);
    void ReservePool(size_t count);

    void SetGroupName(const char *name);

public:
    MaterialAssetsGroup(const char *groupName, const MaterialAssetsProvider *pProvider);
    virtual ~MaterialAssetsGroup();

    const char *Description() const override;

    const std::vector<MaterialAsset *> &Assets() const
    {
        return m_vecAssets;
    }
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

    const std::vector<MaterialAssetsGroup *> &AssetsGroups()
    {
        return m_vecGroups;
    }

    virtual ~MaterialAssetsProvider();
};

typedef std::shared_ptr<MaterialAsset> MaterialAssetPtr;