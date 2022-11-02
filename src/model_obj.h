/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once
#include "file_system.h"
#include "gl_texture.h"
#include "scene_renderer.h"
#include "gl_backend.h"

typedef struct mobjface_s
{
	unsigned short vert;
	unsigned short uv;
	unsigned short norm;

	size_t material_id;
	size_t group_id;
}mobjface_t;

#define MAX_LIGHT_STYLES 4

typedef struct mobjmesh_s
{
	gltexture_t* diffuse_texture;
	gltexture_t* lightmap_texture[MAX_LIGHT_STYLES];

	std::string  diffuse_texture_path;
	std::string  lightmap_texture_path[MAX_LIGHT_STYLES];

	size_t first_face;
	size_t num_faces;

}mobjmesh_t;

class ModelOBJ: public ISelectableObject
{
public:
	ModelOBJ(FileData* pFileData);
	~ModelOBJ();

	void DrawDebug();
	void DrawShaded();

	std::vector<lightDef_t>& ParsedLightDefs();

	std::string GetModelFileName();
	std::string GetModelTextureName();

	void ReloadTextures();
	void Export(const char* fileName);

	void ClearLightDefinitions();
	void AddLight(lightDefPtr_t& it);

	void SetLightmapDimensions(int w, int h);

	DrawMesh* GetDrawMesh();

	float GetSceneScale()
	{
		return m_flSceneScale;
	}


	void OnHovered() override;
	void OnMouseMove(glm::vec2 delta) override;
	void OnSelect() override;
	void OnUnSelect() override;
	void OnUnhovered() override;
	void RenderForSelection(int objectId, class SceneRenderer*) override;

private:
	size_t m_VertSize = 0;
	size_t m_UVSize = 0;

	unsigned int	m_LightsParsed = 0;

	std::vector<float>	m_vecVertsData;
	std::vector<float>	m_vecUVData;
	std::vector<float>	m_vecNormalsData;

	std::vector<mobjface_t>		m_vecFaces;
	std::vector<mobjmesh_t> m_vMaterials;
	std::vector<lightDef_t>		m_vParsedLightDefs;
	
	std::vector<std::string> m_vecGroups;

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
	void ExportVerticles(FILE* fp);
	void ExportNormals(FILE* fp);
	void ExportUV(FILE* fp);
	void ExportFaces(FILE* fp);

	std::string m_strModelName;
	std::string m_strDiffuseName;
	void ParseGroup(std::string& buffer);


	void BuildDrawMesh();
	DrawMesh mesh;

	float m_flSceneScale = 1.f;
};

typedef struct mobjmaterial_s
{
	std::string name = "";

	glm::vec3 Ka = glm::vec3(1);
	glm::vec3 Kd = glm::vec3(1);
	glm::vec3 Ks = glm::vec3(1);

	float d = 1.f;
	float illum = 1.f;

	gltexture_t* map_Ka = nullptr;
	gltexture_t* map_Kd = nullptr;
	gltexture_t* map_Ks = nullptr;

	gltexture_t* map_Ns = nullptr;
	gltexture_t* map_d = nullptr;	
	gltexture_t* map_bump = nullptr;

	gltexture_t* map_disp = nullptr;
	gltexture_t* map_decal = nullptr;

	std::unordered_map<std::string, std::string> extra_values;

}mobjmaterial_t;

enum class MTLTokens
{
	Badtoken = 0,
	NewMaterial,
	Ka,
	Kd,
	Ks,
	Tf,
	Illum,
	d,
	Ns,
	Sharpness,
	Ni,
	MapKa,
	MapKd,
	MapKs,
	MapNs,
	MapD,
	Disp,
	Decal,
	Bump,
	Refl,
	Type,
	Sphere
};

class MaterialTemplateLibrary
{
	
	std::string m_FileName;// canonical
	std::unordered_map<std::string, mobjmaterial_t*> m_LoadedMaterials;
	
	void ParseCommand(std::string& buffer, int lineNumber);
	void ParseFileData(FileData * sourceFile);

	MTLTokens	ParseToken(std::string& token);

public:
	MaterialTemplateLibrary(FileData* sourceFile);
	~MaterialTemplateLibrary();

	mobjmaterial_t* GetByName(const char* name);
	mobjmaterial_t* GetByIndex(const size_t index);
	size_t			MaterialsCount();

	void			ExportToFile(const char* fileName);

private:
	void AddNewMaterial(std::string & name);
};