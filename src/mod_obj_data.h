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

	unsigned int materialId;
	unsigned int groupId;
	unsigned int objectId;
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

	size_t object_id;

	size_t first_vertex = 0;
	size_t num_vertices = 0;

}mobjegroup_t;

typedef struct mobjobjects_s
{
	char name[64] = { 0 };
}mobjobjects_t;

#define FL_DATA_LOADED (1<<0)
#define FL_HAS_UV (1<<1)
#define FL_HAS_NORMALS (1<<2)
#define FL_LOAD_LIGHTMAP (1<<3)

typedef struct mobjdata_s
{
	int flags = 0;

	size_t vertSize = 0;
	size_t uvSize = 0;

	std::vector<float>						verts;
	std::vector<float>						uvs;
	std::vector<float>						normals;

	std::vector<mobjface_t>					faces;
	std::vector<mobjmesh_t>					meshes;
	std::vector<mobjegroup_t>				groups;
	std::vector<mobjobjects_t>				objects;

	std::vector<mobjmaterial_t*>			materials; // owned by material lib from this structure
	std::vector<MaterialTemplateLibrary*>	materialLibs; // owns


	std::vector<SceneEntityPtr>		lightDefs;
		
	glm::vec3					envColor = {1,1,1};
	size_t						lightmapDimensions[2] = {0};
	float						sceneScale = 1.f;
	
	//unsigned int	m_LightsParsed = 0;
	//mobjmaterial_t* mCurrentMaterial = nullptr;
}mobjdata_t;