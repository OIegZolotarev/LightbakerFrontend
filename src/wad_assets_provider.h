/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once
#include "material_assets.h"
#include "wad_textures.h"

class WADAssetsProvider;
class WADAssetsGroups;

class WADMaterialAsset : public MaterialAsset
{
    GoldSource::lumpinfo_t m_LumpInfo;

public:
    WADMaterialAsset(GoldSource::lumpinfo_t *pWadEntry, const WADAssetsGroups * pGroup);
    ~WADMaterialAsset();
};

class WADAssetsGroups : public MaterialAssetsGroup
{
public:
    WADAssetsGroups(const char *wadName, const WADAssetsProvider *pProvider);
};

class WADAssetsProvider: public MaterialAssetsProvider
{
public:
    WADAssetsProvider(const GameConfiguration *pGameConfiguration);
};