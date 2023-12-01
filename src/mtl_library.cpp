/*
	LightBaker3000 Frontend project,
	(c) 2022-2023, CrazyRussian
*/


#include "application.h"
#include "common_resources.h"
#include "gl_backend.h"
#include "mtl_library.h"
#include "text_utils.h"

// MTL Loader

// http://paulbourke.net/dataformats/mtl/

MaterialTemplateLibrary::MaterialTemplateLibrary(FileData* sourceFile)
{
	auto fs = Application::GetFileSystem();
	m_FileName = fs->BaseNameFromPath(sourceFile->Name()) + ".mtl";
	m_Directory = fs->BaseDirectoryFromPath(sourceFile->Name().c_str());

	ParseFileData(sourceFile);
}

void MaterialTemplateLibrary::ParseFileData(FileData* sourceFile)
{
	size_t offset = 0;
	std::string buffer;

	char* p = (char*)sourceFile->Data();

	int lineNumber = 1;

	mobjmaterial_t* current = nullptr;

	AsynchTextureLoadTask* textureLoader = new AsynchTextureLoadTask;

	while (*p)
	{
		if (*p == '\n')
		{
			if (buffer.size() > 0)
				ParseCommand(textureLoader,buffer, lineNumber, current);

			lineNumber++;

			buffer = "";
			p++;
			continue;
		}


		buffer += *p++;
	}

	LoaderThread::Instance()->ScheduleTask(textureLoader);
}

MTLTokens MaterialTemplateLibrary::ParseToken(std::string& token)
{
	std::string lowerCase;
	lowerCase.resize(token.length());

	// Make lower case
	std::transform(token.begin(), token.end(), lowerCase.begin(),
		[](unsigned char c) { return std::tolower(c); });

	typedef std::pair<std::string, MTLTokens> tokenParsePair;

	static std::unordered_map<std::string, MTLTokens> m;

	if (!m.size())
	{
		m.insert(tokenParsePair("newmtl", MTLTokens::NewMaterial));
		m.insert(tokenParsePair("ka", MTLTokens::Ka));
		m.insert(tokenParsePair("kd", MTLTokens::Kd));
		m.insert(tokenParsePair("ks", MTLTokens::Ks));
		m.insert(tokenParsePair("tf", MTLTokens::Tf));
		m.insert(tokenParsePair("illum", MTLTokens::Illum));
		m.insert(tokenParsePair("d", MTLTokens::d));
		m.insert(tokenParsePair("ns", MTLTokens::Ns));
		m.insert(tokenParsePair("sharpness", MTLTokens::Sharpness));
		m.insert(tokenParsePair("ni", MTLTokens::Ni));
		m.insert(tokenParsePair("map_ka", MTLTokens::MapKa));
		m.insert(tokenParsePair("map_kd", MTLTokens::MapKd));
		m.insert(tokenParsePair("map_ks", MTLTokens::MapKs));
		m.insert(tokenParsePair("map_ns", MTLTokens::MapNs));
		m.insert(tokenParsePair("map_d", MTLTokens::MapD));
		m.insert(tokenParsePair("disp", MTLTokens::Disp));
		m.insert(tokenParsePair("decal", MTLTokens::Decal));
		m.insert(tokenParsePair("bump", MTLTokens::Bump));
		m.insert(tokenParsePair("refl", MTLTokens::Refl));
		m.insert(tokenParsePair("type", MTLTokens::Type));
		m.insert(tokenParsePair("sphere", MTLTokens::Sphere));
	}

	auto bucket = m.find(lowerCase);

	if (bucket == m.end())
		return MTLTokens::Badtoken;

	return bucket->second;
}

MaterialTemplateLibrary::~MaterialTemplateLibrary()
{

}

mobjmaterial_t* MaterialTemplateLibrary::GetByName(const char* name)
{
	auto it = m_LoadedMaterials.find(name);
	if (it != m_LoadedMaterials.end())
		return it->second;

	return nullptr;
}

mobjmaterial_t* MaterialTemplateLibrary::GetByIndex(size_t index)
{
	auto it = m_LoadedMaterials.begin();
	std::advance(it, index);
	return it->second;
}

size_t MaterialTemplateLibrary::MaterialsCount()
{
	return m_LoadedMaterials.size();
}

void MaterialTemplateLibrary::ExportToFile(const char* fileName)
{

}

std::string& MaterialTemplateLibrary::ExportName()
{
	return m_FileName;
}

mobjmaterial_t* MaterialTemplateLibrary::AddNewMaterial(std::string& name)
{
	mobjmaterial_t* mat = new mobjmaterial_t;
	mat->name = name;

	m_LoadedMaterials.insert({ name, mat });

	return mat;
}

void MaterialTemplateLibrary::ParseCommand(AsynchTextureLoadTask* textureLoader, std::string& buffer, int lineNumber, mobjmaterial_t*& currentMaterial)
{
	auto tokensString = TextUtils::SpliteWhitespaces(buffer);

	if (!tokensString.size())
		return;

	auto token = ParseToken(tokensString[0]);

#define ASSERT_ARGUMENTS_COUNT(count) if (tokensString.size() < count)  \
		{  \
			Con_Printf("Insufficient arguments (%d, expected %d) for command \"%s\" on line %d\n", tokensString.size(),count,tokensString[0].c_str(),lineNumber); \
			return; \
		}

#define ASSERT_HAS_MATERIAL() if (currentMaterial == nullptr)  \
	{  \
		Con_Printf("Bad  command \"%s\" on line %d - no material defined (so far)\n", buffer.c_str(), lineNumber); \
		return; \
	}

	auto parseVector3 = [](std::vector<std::string>& tokens) -> glm::vec3
	{
		glm::vec3 r;
		r[0] = atof(tokens[1].c_str());
		r[1] = atof(tokens[2].c_str());
		r[2] = atof(tokens[3].c_str());

		return r;
	};

	auto parseVector2 = [](std::vector<std::string>& tokens) -> glm::vec2
	{
		glm::vec2 r;
		r[0] = atof(tokens[1].c_str());
		r[1] = atof(tokens[2].c_str());
		return r;
	};

	auto loadMap = [&](std::vector<std::string>& tokens) -> GLTexture*
	{
		auto fileName = m_Directory + "/" + tokens[1];
		GLTexture* result = textureLoader->ScheduleTexture(fileName.c_str());
		return result;
	};

	switch (token)
	{
	case MTLTokens::Badtoken:
		// Add custom values?
		return;
		break;
	case MTLTokens::NewMaterial:
		ASSERT_ARGUMENTS_COUNT(2);
		currentMaterial = AddNewMaterial(tokensString[1]);
		break;
	case MTLTokens::Ka:
		ASSERT_HAS_MATERIAL();
		ASSERT_ARGUMENTS_COUNT(4);
		currentMaterial->Ka = parseVector3(tokensString);
		break;
	case MTLTokens::Kd:
		ASSERT_HAS_MATERIAL();
		ASSERT_ARGUMENTS_COUNT(4);
		currentMaterial->Ks = parseVector3(tokensString);
		break;
	case MTLTokens::Ks:
		ASSERT_HAS_MATERIAL();
		ASSERT_ARGUMENTS_COUNT(4);
		currentMaterial->Ks = parseVector3(tokensString);
		break;
	case MTLTokens::Tf:
		break;
	case MTLTokens::Illum:
		break;
	case MTLTokens::d:
		break;
	case MTLTokens::Ns:
		break;
	case MTLTokens::Sharpness:
		break;
	case MTLTokens::Ni:
		break;
	case MTLTokens::MapKa:
		ASSERT_HAS_MATERIAL();
		ASSERT_ARGUMENTS_COUNT(2);
		currentMaterial->map_Ka = loadMap(tokensString);
		break;
	case MTLTokens::MapKd:
		ASSERT_HAS_MATERIAL();
		ASSERT_ARGUMENTS_COUNT(2);
		currentMaterial->map_Kd = loadMap(tokensString);
		break;
	case MTLTokens::MapKs:
		ASSERT_HAS_MATERIAL();
		ASSERT_ARGUMENTS_COUNT(2);
		currentMaterial->map_Ks = loadMap(tokensString);
		break;
	case MTLTokens::MapNs:
		ASSERT_HAS_MATERIAL();
		ASSERT_ARGUMENTS_COUNT(2);
		currentMaterial->map_Ns = loadMap(tokensString);
		break;
	case MTLTokens::MapD:
		ASSERT_HAS_MATERIAL();
		ASSERT_ARGUMENTS_COUNT(2);
		currentMaterial->map_d = loadMap(tokensString);
		break;
	case MTLTokens::Disp:
		ASSERT_HAS_MATERIAL();
		ASSERT_ARGUMENTS_COUNT(2);
		currentMaterial->map_disp = loadMap(tokensString);
		break;
	case MTLTokens::Decal:
		ASSERT_HAS_MATERIAL();
		ASSERT_ARGUMENTS_COUNT(2);
		currentMaterial->map_decal = loadMap(tokensString);
		break;
	case MTLTokens::Bump:
		ASSERT_HAS_MATERIAL();
		ASSERT_ARGUMENTS_COUNT(2);
		currentMaterial->map_bump = loadMap(tokensString);
		break;
	case MTLTokens::Refl:
		break;
	case MTLTokens::Type:
		break;
	case MTLTokens::Sphere:
		break;
	default:
		break;

	}

}