/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once
#include "material_assets.h"
#include "wad_textures.h"
#include "goldsource_game_configuration.h"

namespace GoldSource { class HammerGameConfiguration; }

namespace GoldSource
{

class WADAssetsProvider;
class WADAssetsGroup;


class WADMaterialAsset : public MaterialAsset
{
    GoldSource::lumpinfo_t m_LumpInfo;
    
public:
    WADMaterialAsset(const GoldSource::lumpinfo_t *pWadEntry, const WADAssetsGroup *pGroup);
    ~WADMaterialAsset();
};

class WADAssetsGroup : public MaterialAssetsGroup
{
    WADTexturePool *m_pWADPool;

public:
    WADAssetsGroup(const char *wadName, const WADAssetsProvider *pProvider);
    ~WADAssetsGroup();
};

class WADAssetsProvider : public MaterialAssetsProvider
{
public:
    WADAssetsProvider(const GoldSource::HammerGameConfiguration * pGameConfig);    
};

} // namespace GoldSource