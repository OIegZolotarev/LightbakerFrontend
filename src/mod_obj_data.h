/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#pragma once

#define MAX_LIGHT_STYLES 4

typedef struct mobjface_s
{
	unsigned int vert;
	unsigned int uv;
	unsigned int norm;

	unsigned int mesh_id;
	unsigned int group_id;
}mobjface_t;

typedef struct mobjmesh_s
{
	size_t id;
	gltexture_t* diffuse_texture = 0;
	gltexture_t* lightmap_texture[MAX_LIGHT_STYLES] = {0};

	std::string  diffuse_texture_path;
	std::string  lightmap_texture_path[MAX_LIGHT_STYLES];

	size_t first_face = 0;
	size_t num_faces = 0;
	
	mobjmaterial_t* material;

}mobjmesh_t;

typedef struct mobjgroup_s
{	
	char name[64] = { 0 };

	size_t first_mesh = 0;
	size_t num_meshes = 0;

}mobjegroup_t;

typedef struct mobjdata_s
{
	size_t vert_size = 0;
	size_t uv_size = 0;

	std::vector<float>	m_vecVertsData;
	std::vector<float>	m_vecUVData;
	std::vector<float>	m_vecNormalsData;

	std::vector<mobjface_t>		m_vecFaces;
	std::vector<mobjmesh_t>		m_vMeshes;
	std::vector<mobjegroup_t>	m_vGroups;
	
	std::vector<lightDef_t>		m_vParsedLightDefs;
	
	//unsigned int	m_LightsParsed = 0;
	//mobjmaterial_t* mCurrentMaterial = nullptr;
}mobjdata_t;