
#define _CRT_SECURE_NO_WARNINGS

#include "camera.h"
#include "model_obj.h"
#include "application.h"
#include "common_resources.h"
#include <intsafe.h>

// for string delimiter
std::vector<std::string> split(std::string s, std::string delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
	{
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}

bool EmptyString(std::string& s)
{
	for (int i = 0; i < s.length(); i++)
		if (s[i] > 32)
			return false;

	return true;
}

std::vector<std::string> split_whitespaces(std::string & s)
{
	std::string token;
	std::vector<std::string> res;

	const unsigned char* p = (const unsigned char*)s.c_str();

	while (*p)
	{
		if (*p < 33)
		{
			if (!EmptyString(token))
			{
				res.push_back(token);
				token = "";
			}
		}
		else
		{
			token += *p;
		}

		p++;
	}

	return res;
}

ModelOBJ::ModelOBJ(FileData* pFileData)
{
	ParseData(pFileData);

	if (m_vMaterials.size() == 0)
	{
		// Load default material...
		std::string modelDir = Application::GetFileSystem()->ParentPath(pFileData->Name());
		std::string baseName = Application::GetFileSystem()->BaseName(pFileData->Name());

		mobjmaterial_t defaultMaterial;
		memset(&defaultMaterial, 0, sizeof(defaultMaterial));

		auto diffusePath = modelDir + "/" + baseName + ".png";

		m_strModelName = pFileData->Name();
		m_strDiffuseName = diffusePath;

		FileData* data = 0;

		defaultMaterial.diffuse_texture_path = diffusePath;
		data = Application::GetFileSystem()->LoadFile(diffusePath.c_str());

		if (data)
		{
			defaultMaterial.diffuse_texture = LoadGLTexture(data);
			delete data;
		}

		for (int i = 0; i < MAX_LIGHT_STYLES; i++)
		{
			auto lmPath = modelDir + "/" + baseName + "_lightmap_" +  std::to_string(i) + ".png";
			data = Application::GetFileSystem()->LoadFile(lmPath.c_str());

			defaultMaterial.lightmap_texture_path[i] = lmPath;

			if (data)
			{
				defaultMaterial.lightmap_texture[i] = LoadGLTexture(data,true);
				delete data;
			}
		}

		m_vMaterials.push_back(defaultMaterial);

		

	}

	std::qsort(m_vecFaces.data(), m_vecFaces.size(), sizeof(mobjface_t), [](const void* pa, const void* pb) -> int
		{
			const mobjface_t* faceA = static_cast<const mobjface_t*>(pa);
			const mobjface_t* faceB = static_cast<const mobjface_t*>(pb);

			if (faceA->group_id == faceB->group_id)
				return faceA->material_id > faceB->material_id;
			else
				return faceA->group_id > faceB->group_id;
		});

// 	FILE* fp = 0;
// 	fopen_s(&fp,"lights_test.txt", "wt");
// 	ExportLightDefs(fp);
// 	fclose(fp);

}

ModelOBJ::~ModelOBJ()
{
	for (auto& it : m_vMaterials)
	{
		FreeGLTexture(it.diffuse_texture);

		for(int i = 0 ; i < MAX_LIGHT_STYLES; i++)
			FreeGLTexture(it.lightmap_texture[i]);
	}

	FreeVector(m_vecFaces);
	FreeVector(m_vecNormalsData);
	FreeVector(m_vecUVData);
	FreeVector(m_vecVertsData);
	FreeVector(m_vParsedLightDefs);
	FreeVector(m_vecGroups);
}

void ModelOBJ::DrawDebug()
{
	if (m_vecFaces.size() == 0)
		return;
			
	size_t currentMaterial = m_vecFaces[0].material_id;
	mobjmaterial_t* pMaterial = &m_vMaterials[currentMaterial];

	bool bNoTextures = false;

	if (pMaterial->lightmap_texture[0])
	{
		glBindTexture(GL_TEXTURE_2D, pMaterial->lightmap_texture[0]->gl_texnum);
		bNoTextures = false;
	}
	else
		bNoTextures = true;

	if (bNoTextures)
		glDisable(GL_TEXTURE_2D);

	glColor3f(1, 1, 1);

	glBegin(GL_TRIANGLES);
	
	for (auto& face : m_vecFaces)
	{
		if (face.material_id != currentMaterial)
		{
			currentMaterial = face.material_id;
			mobjmaterial_t* pMaterial = &m_vMaterials[currentMaterial];

			if (pMaterial->lightmap_texture[0])
			{
				glBindTexture(GL_TEXTURE_2D, pMaterial->lightmap_texture[0]->gl_texnum);
				bNoTextures = false;
			}
			else
			{
				glDisable(GL_TEXTURE_2D);
			}
		}
		
		glTexCoord2fv(&m_vecUVData[(face.uv- 1) * m_UVSize]);

		if (bNoTextures)
		{
			float* norm = &m_vecNormalsData[(face.norm - 1) * 3];
			glColor3f(
				(norm[0] + 1) / 2,
				(norm[1] + 1) / 2,
				(norm[2] + 1) / 2);
		}
		glVertex3fv(&m_vecVertsData[(face.vert - 1) * m_VertSize]);
	}

	glEnd();


	glEnable(GL_TEXTURE_2D);

	
}

std::vector<lightDef_t>& ModelOBJ::ParsedLightDefs()
{
	return m_vParsedLightDefs;
}

void ModelOBJ::ParseData(FileData* pFileData)
{
	size_t offset = 0;
	std::string buffer;

	char* p = (char*)pFileData->Data();

	while (*p)
	{
		if (*p == '\n')
		{
			if (buffer.size() > 0)
				ParseCommand(buffer);

			buffer = "";
			p++;
			continue;
		}


		buffer += *p++;
	}
}

void ModelOBJ::ParseCommand(std::string& buffer)
{
	char commandId = buffer[0];

	switch (commandId)
	{
	case '#':
		ParseLightDef(buffer);
		break;
	case 'v':
		// Vertex commands
	{
		char subcommand = buffer[1];

		switch (subcommand)
		{
		case 'n':
			ParseNormal(buffer);
			break;
		case 't':
			ParseUV(buffer);
			break;
		default:
			ParseVertex(buffer);
			break;
		}
	}
	break;
	case 'f':
		ParseFace(buffer);
		break;
	case 's':
		// smooth shading
		break;
	case 'o':
		// object
		break;
	case 'g':
		ParseGroup(buffer);
		break;
	case 'u':
		// usemtl
		break;
	case 'm':
		// external mtl
		break;
	}
}

void ModelOBJ::ParseNormal(std::string& s)
{
	size_t pos = 0;
	std::string token;

	int normSize = 0;

	s.erase(0, 2);

	do
	{
		token = s.substr(0, pos);

		if (!EmptyString(token))
		{
			m_vecNormalsData.push_back(stof(token));
			normSize++;
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!EmptyString(token))
	{
		m_vecNormalsData.push_back(stof(token));
		normSize++;
	}

	if (normSize != 3)
	{
		Con_Printf("[ERROR] Normal is not 3 component!");
	}
}

void ModelOBJ::ParseUV(std::string& s)
{
	size_t pos = 0;
	std::string token;

	int newUVSize = 0;

	s.erase(0, 2);

	do
	{
		token = s.substr(0, pos);

		if (!EmptyString(token))
		{
			m_vecUVData.push_back(stof(token));
			newUVSize++;
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!EmptyString(token))
	{
		m_vecUVData.push_back(stof(token));
		newUVSize++;
	}

	if (m_UVSize)
	{
		if (newUVSize != m_UVSize)
		{
			Con_Printf("[WARN] Inconsistent UV size!");
		}
	}
	else
		m_UVSize = newUVSize;
}

void ModelOBJ::ParseVertex(std::string& s)
{
	size_t pos = 0;
	std::string token;

	int newVertSize = 0;

	s.erase(0, 1);

	do
	{
		token = s.substr(0, pos);

		if (!EmptyString(token))
		{
			m_vecVertsData.push_back(stof(token));
			newVertSize++;
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!EmptyString(token))
	{
		m_vecVertsData.push_back(stof(token));
		newVertSize++;
	}

	if (m_VertSize)
	{
		if (newVertSize != m_VertSize)
		{
			Con_Printf("[WARN] Inconsistent vertex size!");
		}
	}
	else
		m_VertSize = newVertSize;
}


void ModelOBJ::ParseGroup(std::string& buffer)
{
	auto tokens = split_whitespaces(buffer);

	if (tokens.size() != 2)
	{
		Con_Printf("Malformed group command: %s\n", buffer.c_str());
		return;
	}

	m_vecGroups.push_back(tokens[1]);
}

void ModelOBJ::ParseFace(std::string& s)
{
	size_t pos = 0; std::string token;

	bool hasUV = m_vecUVData.size() > 0;
	bool hasNorm = m_vecNormalsData.size() > 0;

	auto parseFaceDef = [&]()
	{
		if (token[0] == '\r')
			return;

		mobjface_t face;
		memset(&face, 0, sizeof(face));

		auto elements = split(token, "/");

		if (elements.size() == 0)
		{
			Con_Printf("[ERROR] Malformed \"f\" command");
		}

		face.vert = stoi(elements[0]);

		if (hasUV)
		{
			if (!elements[1].empty())
				face.uv = stoi(elements[1]);
		}

		if (hasNorm)
		{
			if (!elements[2].empty())
				face.norm = stoi(elements[2]);
		}

		face.material_id = m_vMaterials.size();
		face.group_id = m_vecGroups.size();


		m_vecFaces.push_back(face);
	};

	s.erase(0, 1);

	do
	{
		token = s.substr(0, pos);

		if (!EmptyString(token))
		{
			parseFaceDef();
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!EmptyString(token))
	{
		parseFaceDef();
	}


}

void ModelOBJ::ParseLightDef(std::string& buffer)
{
	if (buffer.length() < 2)
		return;

	if (buffer[1] == '#')
		return;
	
	if (m_LightsParsed == 2)
		return;

	auto tokens = split_whitespaces(buffer);

	if (tokens.size() == 0)
		return;

	//#lm_size 1024 1024
	//#env_color 0.3 0.3 0.36

	if (tokens[0] == "#lm_size")
	{
		if (tokens.size() != 3)
		{
			Con_Printf("[ERROR] Malformed #lm_size command");
			return;
		}

		m_LightmapDimensions[0] = stoi(tokens[1]);
		m_LightmapDimensions[1] = stoi(tokens[2]);

		return;
	}

	if (tokens[0] == "#env_color")
	{
		if (tokens.size() != 4)
		{
			Con_Printf("[ERROR] Malformed #env_color command");
			return;
		}

		m_EnvColor[0] = stof(tokens[1]);
		m_EnvColor[1] = stof(tokens[2]);
		m_EnvColor[2] = stof(tokens[3]);

		return;
	}

	// If not found lights section yet, check if we stumbled on one
	if (m_LightsParsed == 0)
	{
		if (tokens[0] == "#lights_start")
			m_LightsParsed = 1;		
		return;
	}

	// Check if section ended
	if (tokens[0] == "#lights_end")
	{
		m_LightsParsed = 2;
		return;
	}

	assert(tokens.size() >= 14);

	lightDef_t newLight;
	memset(&newLight, 0, sizeof(newLight));
	
	// Light type

	auto typeInfo = split(tokens[0], "_");

	if		(typeInfo[0] == "#omni")		newLight.type = LightTypes::Omni;
	else if (typeInfo[0] == "#direct")		newLight.type = LightTypes::Direct;
	else if (typeInfo[0] == "#spot")		newLight.type = LightTypes::Spot;

	newLight.editor_icon = nullptr;
	newLight.UpdateEditorIcon();


	// Parse flags
	for (size_t i = 1; i < typeInfo.size(); i++)
	{
		if (typeInfo[i] == "euler")
			newLight.flags |= LF_EULER;
		else if (typeInfo[i] == "dyn")
			newLight.flags |= LF_EULER;
		else if (typeInfo[i] == "xyz")
			newLight.flags |= LF_XYZ;
		else if (typeInfo[i] == "linear")
			newLight.flags |= LF_XYZ;
		else if (typeInfo[i] == "disk")
			newLight.flags |= LF_DISK;
		else if (typeInfo[i] == "rect")
			newLight.flags |= LF_RECT;
	}

	newLight.pos[0] = stof(tokens[1]);
	newLight.pos[1] = stof(tokens[2]);
	newLight.pos[2] = stof(tokens[3]);

	newLight.color[0] = stof(tokens[4]);
	newLight.color[1] = stof(tokens[5]);
	newLight.color[2] = stof(tokens[6]);

	newLight.intensity = stof(tokens[7]);

	newLight.anglesDirection[0] = stof(tokens[8]);
	newLight.anglesDirection[1] = stof(tokens[9]);
	newLight.anglesDirection[2] = stof(tokens[10]);

	newLight.cones[0] = stof(tokens[11]);
	newLight.cones[1] = stof(tokens[12]);

	newLight.style = stoi(tokens[13]);

	if (tokens.size() > 14)
		newLight.size[0] = stof(tokens[14]);
	else
		newLight.size[0] = 0;

	if (tokens.size() > 15)
		newLight.size[1] = stof(tokens[15]);
	else
		newLight.size[1] = newLight.size[0];

	m_vParsedLightDefs.push_back(newLight);
}

void ModelOBJ::ExportLightDefs(FILE* fp)
{
	fprintf(fp, "### LightBaker 3000 lights definition\n\n");
	
	fprintf(fp, "#lm_size %d %d\n", m_LightmapDimensions[0], m_LightmapDimensions[1]);
	fprintf(fp, "#env_color %.3f %.3f %.3f\n\n", m_EnvColor[0],m_EnvColor[1],m_EnvColor[1]);

	fprintf(fp, "#lights_start\n");

	for (auto& it : m_vParsedLightDefs)
	{
		std::string lightBaseType;

		switch (it.type)
		{
		case LightTypes::Omni:
			lightBaseType = "#omni";
			break;
		case LightTypes::Spot:
			lightBaseType = "#spot";
			break;
		case LightTypes::Direct:
			lightBaseType = "#direct";
			break;
		default:
			Application::EPICFAIL("Unknown light type %d !", it.type);
			break;
		}

		if (it.flags & LF_EULER)	lightBaseType += "_euler";
		if (it.flags & LF_DYN)		lightBaseType += "_dyn";
		if (it.flags & LF_XYZ)		lightBaseType += "_xyz";
		if (it.flags & LF_LINEAR)	lightBaseType += "_linear";
		if (it.flags & LF_DISK)		lightBaseType += "_disk";
		if (it.flags & LF_RECT)		lightBaseType += "_rect";


		fprintf(fp,"%s\t%.3f %.3f %.3f\t%.3f %.3f %.3f\t%.3f\t%.3f %.3f %.3f\t%.3f %.3f\t%d\t",
				lightBaseType.c_str(),
				it.pos[0], it.pos[1], it.pos[2],
				it.color[0], it.color[1], it.color[2],
				it.intensity,
				it.anglesDirection[0], it.anglesDirection[1], it.anglesDirection[2],
				it.cones[0], it.cones[1],
				it.style);

		switch (it.type)
		{
		case LightTypes::Omni:
		case LightTypes::Spot:
			if (it.flags & LF_RECT)
			{
				fprintf(fp, "%.3f %.3f\n",it.size[0],it.size[1]);
			}
			else
				fprintf(fp, "%.3f\n", it.size[0]);
					
			break;
		case LightTypes::Direct:
			fprintf(fp, "\n");
			break;
		default:
			break;

		}
	}

	fprintf(fp, "#lights_end\n");
}

void ModelOBJ::ExportVerticles(FILE* fp)
{
	fprintf(fp, "\n# %d verticles start\n", m_vecVertsData.size() / m_VertSize);

	size_t offset = 0;

	while (offset < m_vecVertsData.size())
	{
		fprintf(fp,"v  ");

		for (size_t i = 0; i < m_VertSize; i++)
		{
			if (i == (m_VertSize - 1))
				fprintf(fp, "%.4f", m_vecVertsData[offset + i]);
			else				
				fprintf(fp, "%.4f ", m_vecVertsData[offset + i]);
		}

		fprintf(fp, "\n");

		offset += m_VertSize;
	}

	fprintf(fp, "\n# %d verticles end\n", m_vecVertsData.size() / m_VertSize);
}

void ModelOBJ::ExportNormals(FILE* fp)
{
	fprintf(fp, "\n# %d normals start\n", m_vecNormalsData.size() / 3);

	size_t offset = 0;

	while (offset < m_vecNormalsData.size())
	{
		float* f = &m_vecNormalsData[offset];

		fprintf(fp, "vn %.4f %.4f %.4f\n",f[0],f[1],f[2]);
		offset += 3;
	}

	fprintf(fp, "\n# %d normals end\n", m_vecNormalsData.size() / 3);
}

void ModelOBJ::ExportUV(FILE* fp)
{
	fprintf(fp, "\n# %d UV start\n", m_vecUVData.size() / m_UVSize);

	size_t offset = 0;

	while (offset < m_vecUVData.size())
	{
		float* f = &m_vecUVData[offset];

		fprintf(fp, "vt  ");

		for (size_t i = 0; i < m_UVSize; i++)
		{
			if (i == m_UVSize - 1)
				fprintf(fp, "%.4f", m_vecUVData[offset + i]);
			else
				fprintf(fp, "%.4f ", m_vecUVData[offset + i]);
		}

		fprintf(fp, "\n");


		offset += m_UVSize;
	}

	fprintf(fp, "\n# %d UV end\n", m_vecUVData.size() / m_UVSize);
}

void ModelOBJ::ExportFaces(FILE* fp)
{
	fprintf(fp, "\n# %d faces start\n", m_vecFaces.size() / 3);

	size_t lastGroup = 0;

	for (size_t i = 0; i < m_vecFaces.size(); i += 3)
	{
		if (m_vecFaces[i].group_id != lastGroup)
		{
			lastGroup = m_vecFaces[i].group_id;
			fprintf(fp, "g %s\n", m_vecGroups[lastGroup - 1].c_str());
		}

		fprintf(fp, "f ");

		for (int j = 0; j < 3; j++)
		{
			auto & f = m_vecFaces[i + j];
			
			if (f.vert)
				fprintf(fp,"%d",f.vert);

			if (f.uv)
				fprintf(fp, "/%d", f.uv);

			if (f.norm)
			{
				if (!f.uv)
					fprintf(fp, "//%d", f.norm);
				else
					fprintf(fp, "/%d", f.norm);
			}

			fprintf(fp, "  ");
		}

		fprintf(fp, " \n");
	}

	fprintf(fp, "\n# %d faces end", m_vecFaces.size() / 3);
}


void ModelOBJ::ClearLightDefinitions()
{
	m_vParsedLightDefs.clear();
}

void ModelOBJ::AddLight(lightDefPtr_t& it)
{
	m_vParsedLightDefs.push_back(*it);
}

void ModelOBJ::ReloadTextures()
{
	for (auto& it : m_vMaterials)
	{
		if (it.diffuse_texture)
			GLReloadTexture(it.diffuse_texture);
		else
		{
			FileData* pData = Application::GetFileSystem()->LoadFile(it.diffuse_texture_path);

			if (pData)
			{
				it.diffuse_texture = LoadGLTexture(pData);
				delete pData;
			}
			
		}

		for (int i = 0; i < MAX_LIGHT_STYLES; i++)
		{
			if (it.lightmap_texture[i])
				GLReloadTexture(it.lightmap_texture[i]);
			else
			{
				FileData* pData = Application::GetFileSystem()->LoadFile(it.lightmap_texture_path[i]);

				if (pData)
				{
					it.lightmap_texture[i] = LoadGLTexture(pData);
					delete pData;
				}
			}
		}
			
	}
}

void ModelOBJ::Export(const char* fileName)
{
	FILE* fp = nullptr;
	fp = fopen(fileName, "wt");

	if (!fp)
		Application::EPICFAIL("Error while opening file %s for exporting!", fileName);

	ExportLightDefs(fp);
	ExportVerticles(fp);
	ExportNormals(fp);
	ExportUV(fp);
	ExportFaces(fp);

	fclose(fp);
}

std::string ModelOBJ::GetModelFileName()
{
	return m_strModelName;
}

std::string ModelOBJ::GetModelTextureName()
{
	return m_strDiffuseName;
}
