#pragma once

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

	void ParseCommand(AsynchTextureLoadTask * textureLoader, std::string& buffer, int lineNumber, mobjmaterial_t*& currentMaterial);
	void ParseFileData(FileData* sourceFile);

	MTLTokens	ParseToken(std::string& token);

public:
	MaterialTemplateLibrary(FileData* sourceFile);
	~MaterialTemplateLibrary();

	mobjmaterial_t* GetByName(const char* name);
	mobjmaterial_t* GetByIndex(const size_t index);
	size_t			MaterialsCount();

	void			ExportToFile(const char* fileName);

private:
	mobjmaterial_t* AddNewMaterial(std::string& name);
	std::string m_Directory;
};




