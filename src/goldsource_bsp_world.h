/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#pragma once

#include "goldsource_bsp_disk_structs.h"
#include "goldsource_bsp_mem_structs.h"


namespace GoldSource
{

class BSPWorld
{
	int m_Version;

	std::vector<mvertex_t> m_vVertices;
	std::vector<medge_t> m_vEdges;
	std::vector<texture_t*> m_vTextures;
	std::vector<mplane_t> m_vPlanes;
	std::vector<mtexinfo_t> m_vTexInfo;
	std::vector<int> m_vSurfedges;
	std::vector<msurface_t> m_vFaces;
	std::vector<msurface_t*> m_vMarkSurfaces;
	std::vector<mleaf_t> m_vLeafs;
	std::vector<mnode_t> m_vNodes;
	std::vector<dclipnode_t> m_vClipNodes;
	std::vector<dmodel_t> m_vSubmodels;
	
	dheader_t* m_Header;
	byte* m_pVisdata = nullptr;
	byte* m_pLightData = nullptr;
	FileData* m_pFileData = nullptr;
	

	void Mod_LoadVertexes(lump_t* l);
	void Mod_LoadEdges(lump_t* l);
	void Mod_LoadSurfedges(lump_t* l);
	void Mod_LoadTextures(lump_t* l);

	void SequenceTextureAnims(dmiptexlump_t* m, texture_t*& tx, texture_t** anims, texture_t** altanims, int& max, int& altmax, int& j, texture_t*& tx2, int& num);

	void Mod_LoadLighting(lump_t* l);
	void Mod_LoadPlanes(lump_t* l);
	void Mod_LoadTexinfo(lump_t* l);
	void Mod_LoadFaces(lump_t* l);
	void Mod_LoadMarksurfaces(lump_t* l);
	void Mod_LoadVisibility(lump_t* l);
	void Mod_LoadLeafs(lump_t* l);
	void Mod_LoadNodes(lump_t* l);
	void Mod_LoadClipnodes(lump_t* l);
	void Mod_LoadEntities(lump_t* l);
	void Mod_LoadSubmodels(lump_t* l);

	void CalcSurfaceExtents(msurface_t* s);

	void Mod_SetParent(mnode_t* node, mnode_t* parent);


public:
	BSPWorld(FileData* fd);
	~BSPWorld();
	std::string GetBaseName();

};

class FunnyLumpSizeException : public std::exception
{
	std::string message;
	BSPWorld* m_pLevel;

public:
	FunnyLumpSizeException(const char* function, BSPWorld* level);
	~FunnyLumpSizeException();
};

}
