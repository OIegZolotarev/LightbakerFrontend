#pragma once

typedef struct mobjmaterial_s
{
	std::string name = "";

	glm::vec3 Ka = glm::vec3(1);
	glm::vec3 Kd = glm::vec3(1);
	glm::vec3 Ks = glm::vec3(1);

	float d = 1.f;
	float illum = 1.f;

	GLTexture* map_Ka = nullptr;
	GLTexture* map_Kd = nullptr;
	GLTexture* map_Ks = nullptr;

	GLTexture* map_Ns = nullptr;
	GLTexture* map_d = nullptr;
	GLTexture* map_bump = nullptr;

	GLTexture* map_disp = nullptr;
	GLTexture* map_decal = nullptr;

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

	std::string& ExportName();

private:
	mobjmaterial_t* AddNewMaterial(std::string& name);
	std::string m_Directory;
};




