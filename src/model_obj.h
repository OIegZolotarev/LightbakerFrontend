#pragma once
#include "file_system.h"
#include "gl_texture.h"
#include "scene_renderer.h"

typedef struct mobjface_s
{
	unsigned short vert;
	unsigned short uv;
	unsigned short norm;

	size_t material_id;
}mobjface_t;

typedef struct mobjmaterial_s
{
	gltexture_t* diffuse_texture;
	gltexture_t* lightmap_texture;
}mobjmaterial_t;

class ModelOBJ
{
public:
	ModelOBJ(FileData* pFileData);
	~ModelOBJ();

	void DrawDebug();
private:
	size_t m_VertSize = 0;
	size_t m_UVSize = 0;

	unsigned int	m_LightsParsed = 0;

	std::vector<float>	m_vecVertsData;
	std::vector<float>	m_vecUVData;
	std::vector<float>	m_vecNormalsData;

	std::vector<mobjface_t>		m_vecFaces;
	std::vector<mobjmaterial_t> m_vMaterials;
	std::vector<lightDef_t>		m_vParsedLightDefs;

	void ParseData(FileData* pFileData);
	void ParseCommand(std::string & buffer);
	void ParseNormal(std::string& buffer);
	void ParseUV(std::string& buffer);
	void ParseVertex(std::string& buffer);
	void ParseFace(std::string& buffer);

	void ParseLightDef(std::string& buffer);
	int m_LightmapDimensions[2];
	glm::vec3 m_EnvColor;

	void ExportLightDefs(FILE* fp);
};

