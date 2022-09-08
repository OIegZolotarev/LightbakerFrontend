#include "camera.h"
#include "model_obj.h"
#include "application.h"

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

std::vector<std::string> split_whitespaces(std::string & s)
{
	std::string token;
	std::vector<std::string> res;

	const unsigned char* p = (const unsigned char*)s.c_str();

	while (*p)
	{
		if (*p < 33)
		{
			if (!token.empty())
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

		FileData* data = 0;

		data = Application::GetFileSystem()->LoadFile(diffusePath.c_str());

		if (data)
			defaultMaterial.diffuse_texture = LoadGLTexture(data);

		auto lmPath = modelDir + "/" + baseName + "_lightmap_0.png";
		data = Application::GetFileSystem()->LoadFile(lmPath.c_str());

		if (data)
			defaultMaterial.lightmap_texture = LoadGLTexture(data);

		m_vMaterials.push_back(defaultMaterial);

	}

	std::qsort(m_vecFaces.data(), m_vecFaces.size(), sizeof(mobjface_t), [](const void* pa, const void* pb) -> int
		{
			const mobjface_t* faceA = static_cast<const mobjface_t*>(pa);
			const mobjface_t* faceB = static_cast<const mobjface_t*>(pb);

			return faceA->material_id > faceB->material_id;
		});

}

ModelOBJ::~ModelOBJ()
{
	for (auto& it : m_vMaterials)
	{
		FreeGLTexture(it.diffuse_texture);
		FreeGLTexture(it.lightmap_texture);
	}

	FreeVector(m_vecFaces);
	FreeVector(m_vecNormalsData);
	FreeVector(m_vecUVData);
	FreeVector(m_vecVertsData);
	FreeVector(m_vParsedLightDefs);

}

void ModelOBJ::DrawDebug()
{
	if (m_vecFaces.size() == 0)
		return;
	
	glRotatef(90, 1, 0, 0);
		
	size_t currentMaterial = m_vecFaces[0].material_id;
	mobjmaterial_t* pMaterial = &m_vMaterials[currentMaterial];

	if (pMaterial->lightmap_texture)
		glBindTexture(GL_TEXTURE_2D,pMaterial->lightmap_texture->gl_texnum);

	glColor3f(1, 1, 1);

	glBegin(GL_TRIANGLES);
	
	for (auto& face : m_vecFaces)
	{
		if (face.material_id != currentMaterial)
		{
			currentMaterial = face.material_id;
			mobjmaterial_t* pMaterial = &m_vMaterials[currentMaterial];

			if (pMaterial->lightmap_texture)
				glBindTexture(GL_TEXTURE_2D, pMaterial->lightmap_texture->gl_texnum);
		}
		
		glTexCoord2fv(&m_vecUVData[(face.uv- 1) * m_UVSize]);
		//glColor3fv(&m_vecVertsData[(face.norm - 1) * 3]);		
		glVertex3fv(&m_vecVertsData[(face.vert - 1) * m_VertSize]);
	}

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPointSize(10);

	glBegin(GL_POINTS);

	for (auto it : m_vParsedLightDefs)
	{
		glColor3fv(&it.color[0]);
		glVertex3fv(&it.pos[0]);
	}

	glEnd();
	glEnable(GL_TEXTURE_2D);

	glRotatef(-90, 1, 0, 0);
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
		// group
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

		if (!token.empty())
		{
			m_vecNormalsData.push_back(stof(token));
			normSize++;
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!token.empty())
	{
		m_vecNormalsData.push_back(stof(token));
		normSize++;
	}

	if (normSize != 3)
	{
		// TODO: error message
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

		if (!token.empty())
		{
			m_vecUVData.push_back(stof(token));
			newUVSize++;
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!token.empty())
	{
		m_vecUVData.push_back(stof(token));
		newUVSize++;
	}

	if (m_UVSize)
	{
		if (newUVSize != m_UVSize)
		{
			// Inconsistent size...
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

		if (!token.empty())
		{
			m_vecVertsData.push_back(stof(token));
			newVertSize++;
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!token.empty())
	{
		m_vecVertsData.push_back(stof(token));
		newVertSize++;
	}

	if (m_VertSize)
	{
		if (newVertSize != m_VertSize)
		{
			// Inconsistent size...
		}
	}
	else
		m_VertSize = newVertSize;
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
			Application::EPICFAIL("Error while parsing obj");
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

		m_vecFaces.push_back(face);
	};

	s.erase(0, 1);

	do
	{
		token = s.substr(0, pos);

		if (!token.empty())
		{
			parseFaceDef();
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!token.empty())
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
			// Con_Printf("Malformed #lm_size command");
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
			// Con_Printf("Malformed #env_color command");
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
	else if (typeInfo[0] == "#direct")	newLight.type = LightTypes::Direct;
	else if (typeInfo[0] == "#spot")		newLight.type = LightTypes::Spot;

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
	fprintf(fp, "#env_color %f %f %f\n\n", m_EnvColor[0],m_EnvColor[1],m_EnvColor[1]);

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


		fprintf(fp,"%s\t\t\t%f %f %f\t\t\t%f %f %f\t\t\t%f\t\t\t%f %f %f\t\t\t%f %f\t\t\t%d\t\t\t",
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
				fprintf(fp, "%f %f\n",it.size[0],it.size[1]);
			}
			else
				fprintf(fp, "%f\n", it.size[0]);
					
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
