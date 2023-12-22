/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "goldsource_bsp_disk_structs.h"
#include "goldsource_bsp_mem_structs.h"
#include "goldsource_lightmap_atlas.h"

class Scene;

namespace GoldSource
{
class BSPLevel
{
    friend class BSPAsynchLoader;
    friend class BSPRenderer;

    bool m_bIsDataLoaded = false;
    int m_Version;

    std::vector<mvertex_t> m_vVertices;
    std::vector<medge_t> m_vEdges;
    std::vector<mtexture_t *> m_vTextures;
    std::vector<mplane_t> m_vPlanes;
    std::vector<mtexinfo_t> m_vTexInfo;
    std::vector<int> m_vSurfedges;
    std::vector<msurface_t> m_vFaces;
    std::vector<msurface_t *> m_vMarkSurfaces;
    std::vector<mleaf_t> m_vLeafs;
    std::vector<mnode_t> m_vNodes;
    std::vector<dclipnode_t> m_vClipNodes;
    std::vector<dmodel_t> m_vSubmodels;

    std::vector<class BSPEntity *> m_vEntities;

    dheader_t *m_Header;
    byte *m_pVisdata      = nullptr;
    byte *m_pLightData    = nullptr;
    char *m_pEntdata      = nullptr;
    FileData *m_pFileData = nullptr;

    LightmapAtlas *m_pLightmapState = nullptr;

    void Mod_LoadVertexes(lump_t *l);
    void Mod_LoadEdges(lump_t *l);
    void Mod_LoadSurfedges(lump_t *l);
    void Mod_LoadTextures(lump_t *l);

    void SequenceTextureAnims(dmiptexlump_t *m, mtexture_t *&tx, mtexture_t **anims, mtexture_t **altanims, int &max,
                              int &altmax, int &j, mtexture_t *&tx2, int &num);

    void Mod_LoadLighting(lump_t *l);
    void Mod_LoadPlanes(lump_t *l);
    void Mod_LoadTexinfo(lump_t *l);
    void Mod_LoadFaces(lump_t *l);
    void Mod_LoadMarksurfaces(lump_t *l);
    void Mod_LoadVisibility(lump_t *l);
    void Mod_LoadLeafs(lump_t *l);
    void Mod_LoadNodes(lump_t *l);
    void Mod_LoadClipnodes(lump_t *l);
    void Mod_LoadSubmodels(lump_t *l);
    void Mod_LoadEntities(lump_t *l);

    void CalcSurfaceExtents(msurface_t *s);

    void Mod_SetParent(mnode_t *node, mnode_t *parent);

    void BuildSurfaceDisplayList(msurface_t *fa);
    void GL_CreateSurfaceLightmap(msurface_t *surf);

    Scene *m_pScene;

  public:


    BSPLevel(FileData *fd, Scene* pScene);
    ~BSPLevel();

    std::string GetBaseName();

    msurface_t *Faces(size_t firstSurface);
    mnode_t *GetNodes(size_t index);

    void Mod_ReloadFacesLighting(lump_t *facesLump);
    void BuildLightMap(msurface_t *surf, byte *base, size_t stride);

    void PopulateScene(Scene * pScene);

    std::vector<msurface_t> &GetFaces();

    std::string Export(const char *newPath, lightBakerSettings_t *lb3kOptions);
    void ReloadLightmaps();

    const std::vector<dmodel_t> &GetSubmodels() const;
    const std::vector<medge_t> &GetEdges() const;
    const std::vector<mvertex_t> &GetVertices() const;
    const std::vector<int> &GetSurfEdges() const;
    LightmapAtlas *GetLightmapState() const;
};

class FunnyLumpSizeException : public std::exception
{
    std::string message;
    BSPLevel *m_pLevel;

  public:
    FunnyLumpSizeException(const char *function, BSPLevel *level);
    ~FunnyLumpSizeException();
};

} // namespace GoldSource
