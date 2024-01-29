/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/


#include "application.h"
#include "common.h"

#include "wad_assets_provider.h"



 WADMaterialAsset::WADMaterialAsset(GoldSource::lumpinfo_t *pWadEntry, const WADAssetsGroups *pGroup)
    : MaterialAsset(pWadEntry->name, pGroup)
{
}

WADMaterialAsset::~WADMaterialAsset()
{
}

WADAssetsGroups::WADAssetsGroups(const char *wadName, const WADAssetsProvider *pProvider) : MaterialAssetsGroup(wadName, pProvider)
{
}

WADAssetsProvider::WADAssetsProvider(const GameConfiguration *pGameConfiguration) : MaterialAssetsProvider(pGameConfiguration)
{

}
