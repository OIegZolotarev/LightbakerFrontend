/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "wad_assets_provider.h"
#include "wad_textures.h"
#include "fs_core.h"


using namespace GoldSource;

WADMaterialAsset::WADMaterialAsset(const GoldSource::lumpinfo_t *pWadEntry, const WADAssetsGroup *pGroup)
    : MaterialAsset(pWadEntry->name, pGroup)
{
    memcpy(&m_LumpInfo, pWadEntry, sizeof(lumpinfo_t));
    
    GLTexture *pDiffuseImage = TextureManager::Instance()->LoadWADTextureAsynch(pWadEntry->name);

    SetDiffuseImage(pDiffuseImage);
    SetEditorImage(pDiffuseImage);
}

WADMaterialAsset::~WADMaterialAsset()
{
}

WADAssetsGroup::WADAssetsGroup(const char *wadName, const WADAssetsProvider *pProvider): MaterialAssetsGroup(nullptr, pProvider)
{
    SetGroupName(FileSystem::ExtractFileName(wadName).c_str());

    m_pWADPool = TextureManager::Instance()->RegisterWAD(wadName, true);

    ReservePool(m_pWADPool->NumItems());

    const lumpinfo_t *pWadItems = m_pWADPool->Items();

    for (size_t i = 0 ;i < m_pWADPool->NumItems(); i++)
    {
        const lumpinfo_t *item = &pWadItems[i];
        
        AddAsset(new WADMaterialAsset(item, this));
    }

}

WADAssetsGroup::~WADAssetsGroup()
{    
    TextureManager::Instance()->UnregisterWAD(m_pWADPool);
    delete m_pWADPool;

    // Loaded assets are managed by parent class
}

WADAssetsProvider::WADAssetsProvider(const GoldSource::HammerGameConfiguration *pGameConfig)
    : MaterialAssetsProvider(pGameConfig)
{
    for (auto & wadFile: pGameConfig->GetWADFiles())
    {
        AddAssetsGroup(new WADAssetsGroup(wadFile.c_str(), this));
    }
}