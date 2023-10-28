/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "lights.h"
#include "text_utils.h"
#include "mod_obj_asynch_loader.h"

void ModObjAsynchLoader::ParseVertex(std::string& s)
{
	size_t pos = 0;
	std::string token;

	int newVertSize = 0;

	s.erase(0, 1);

	do
	{
		token = s.substr(0, pos);

		if (!TextUtils::EmptyString(token))
		{
			m_Data->verts.push_back(stof(token));
			newVertSize++;
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!TextUtils::EmptyString(token))
	{
		m_Data->verts.push_back(stof(token));
		newVertSize++;
	}

	if (m_Data->vertSize)
	{
		if (newVertSize != m_Data->vertSize)
		{
			Con_Printf("[WARN] Inconsistent vertex size!");
		}
	}
	else
		m_Data->vertSize = newVertSize;
}

void ModObjAsynchLoader::ParseUV(std::string& s)
{
	size_t pos = 0;
	std::string token;

	int newUVSize = 0;

	s.erase(0, 2);

	do
	{
		token = s.substr(0, pos);

		if (!TextUtils::EmptyString(token))
		{
			m_Data->uvs.push_back(stof(token));
			newUVSize++;
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!TextUtils::EmptyString(token))
	{
		m_Data->uvs.push_back(stof(token));
		newUVSize++;
	}

	if (m_Data->uvSize)
	{
		if (newUVSize != m_Data->uvSize)
		{
			Con_Printf("[WARN] Inconsistent UV size!");
		}
	}
	else
		m_Data->uvSize = newUVSize;

	m_Data->flags |= FL_HAS_UV;
}

void ModObjAsynchLoader::ParseNormal(std::string& s)
{
	size_t pos = 0;
	std::string token;

	int normSize = 0;

	s.erase(0, 2);

	do
	{
		token = s.substr(0, pos);

		if (!TextUtils::EmptyString(token))
		{
			m_Data->normals.push_back(stof(token));
			normSize++;
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!TextUtils::EmptyString(token))
	{
		m_Data->normals.push_back(stof(token));
		normSize++;
	}

	if (normSize != 3)
	{
		Con_Printf("[ERROR] Got a malformed normal - expected 3 components, got %d", normSize);
	}

	m_Data->flags |= FL_HAS_NORMALS;
}

void ModObjAsynchLoader::ParseLightDef(std::string& buffer)
{
	if (buffer.length() < 2)
		return;

	if (buffer[1] == '#')
		return;

	if (m_LightsParsingStage == LightParsingStage::DoneParsing)
		return;

	auto tokens = TextUtils::SpliteWhitespaces(buffer);

	if (tokens.size() == 0)
		return;

	//#lm_size 1024 1024
	//#env_color 0.3 0.3 0.36

	if (tokens[0] == "#lm_size")
	{
		if (tokens.size() != 3)
		{
			Con_Printf("[ERROR] Malformed #lm_size command\n");
			return;
		}

		m_Data->lightmapDimensions[0] = stoi(tokens[1]);
		m_Data->lightmapDimensions[1] = stoi(tokens[2]);

		return;
	}

	if (tokens[0] == "#scene_scale")
	{
		if (tokens.size() != 2)
		{
			Con_Printf("[ERROR] Malformed #scene_scale command\n");
			return;
		}

		m_Data->sceneScale = stof(tokens[1]);
		return;
	}

	if (tokens[0] == "#env_color")
	{
		if (tokens.size() != 4)
		{
			Con_Printf("[ERROR] Malformed #env_color command\n");
			return;
		}

		m_Data->envColor[0] = stof(tokens[1]);
		m_Data->envColor[1] = stof(tokens[2]);
		m_Data->envColor[2] = stof(tokens[3]);

		return;
	}

	// If not found lights section yet, check if we stumbled on one
	if (m_LightsParsingStage == LightParsingStage::NotEncountered)
	{
		if (tokens[0] == "#lights_start")
			m_LightsParsingStage = LightParsingStage::Parsing;
		return;
	}

	// Check if section ended
	if (tokens[0] == "#lights_end")
	{
		m_LightsParsingStage = LightParsingStage::DoneParsing;
		return;
	}

	assert(tokens.size() >= 14);

	lightDef_t newLight;
	memset(&newLight, 0, sizeof(newLight));

	// Light type

	auto typeInfo = TextUtils::Split(tokens[0], "_");

	if (typeInfo[0] == "#omni")		newLight.type = LightTypes::Omni;
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

	newLight.pos[0] = stof(tokens[1]) * m_Data->sceneScale;
	newLight.pos[1] = stof(tokens[2]) * m_Data->sceneScale;
	newLight.pos[2] = stof(tokens[3]) * m_Data->sceneScale;

	newLight.color[0] = stof(tokens[4]);
	newLight.color[1] = stof(tokens[5]);
	newLight.color[2] = stof(tokens[6]);

	newLight.intensity = stof(tokens[7]) * m_Data->sceneScale;

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

	m_Data->lightDefs.push_back(newLight);
}

void ModObjAsynchLoader::ParseFace(std::string& s)
{
	size_t pos = 0; std::string token;
	
	size_t groupId = CurrentResource(StateMachineResource::Group);
	size_t materialId = CurrentResource(StateMachineResource::Material);
	size_t objectId = CurrentResource(StateMachineResource::Object);
	
	bool hasUV = m_Data->flags & FL_HAS_UV;
	bool hasNorm = m_Data->flags & FL_HAS_NORMALS;

	std::vector<mobjface_t> parsed;

	auto parseFaceDef = [&]()
	{
		if (token[0] == '\r')
			return;

		mobjface_t face;
		memset(&face, 0, sizeof(face));

		auto elements = TextUtils::Split(token, "/");

		if (elements.size() == 0)
		{
			Con_Printf("[ERROR] Malformed \"f\" command");
		}

		face.vert = stoi(elements[0]);

		assert(face.vert != 0);

		if (hasUV && elements.size() > 1)
		{
			if (!elements[1].empty())
				face.uv = stoi(elements[1]);
		}

		if (hasNorm && elements.size() > 2)
		{
			if (!elements[2].empty())
				face.norm = stoi(elements[2]);
		}

		face.materialId = materialId;
		face.groupId = groupId;
		face.objectId = objectId;

		parsed.push_back(face);
	};

	s.erase(0, 1);

	do
	{
		token = s.substr(0, pos);

		if (!TextUtils::EmptyString(token))
		{
			parseFaceDef();
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!TextUtils::EmptyString(token))
	{
		parseFaceDef();
	}

	// Triangulate

	for (int i = 0; i < parsed.size() - 2; i++)
	{
		m_Data->faces.push_back(parsed[0]);
		m_Data->faces.push_back(parsed[i + 1]);
		m_Data->faces.push_back(parsed[i + 2]);
	}
}

void ModObjAsynchLoader::ParseGroup(std::string& buffer)
{
	auto tokens = TextUtils::SpliteWhitespaces(buffer);

	if (tokens.size() != 2)
	{
		Con_Printf("Malformed group command: %s\n", buffer.c_str());
		return;
	}

	if (m_Data->groups.size() > 0)
	{
		mobjegroup_t& grp = m_Data->groups[m_Data->groups.size() - 1];
		grp.num_faces = m_Data->groups.size() - grp.first_face;
	}

	mobjegroup_t grp;
	strncpy_s(grp.name, tokens[1].c_str(), sizeof(grp.name) - 1);

	grp.object_id = CurrentResource(StateMachineResource::Object);
	grp.first_face = m_Data->faces.size();
	
	m_Data->groups.push_back(grp);
}


void ModObjAsynchLoader::ParseObject(std::string& buffer)
{
	auto tokens = TextUtils::SpliteWhitespaces(buffer);

	if (tokens.size() != 2)
	{
		Con_Printf("Malformed object command: %s\n", buffer.c_str());
		return;
	}

	mobjobjects_t obj;
	strncpy_s(obj.name, tokens[1].c_str(), sizeof(obj.name) - 1);

	m_Data->objects.push_back(obj);
}


void ModObjAsynchLoader::ParseMaterialLib(std::string& buffer)
{
	auto tokens = TextUtils::SpliteWhitespaces(buffer);

	if (tokens.size() != 2)
	{
		// TODO: ����� � ��������� ������ ������
		Con_Printf("Malformed mtllib command: %s\n", buffer.c_str());
		return;
	}

	auto baseDir = m_pFileData->DirName();

	// TODO: check if extension is needed sometimes
	auto mtlName = baseDir + "/" + tokens[1];
	auto data = Application::GetFileSystem()->LoadFile(mtlName);

	if (data != nullptr)
	{
		auto materialLib = new MaterialTemplateLibrary(data);
		m_Data->materialLibs.push_back(materialLib);
		data->UnRef();
	}

}

void ModObjAsynchLoader::ParseCommand(std::string& buffer)
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
		ParseObject(buffer);
		break;
	case 'g':
		ParseGroup(buffer);
		break;
	case 'u':
		ParseUseMtl(buffer);
		break;
	case 'm':
		ParseMaterialLib(buffer);
		break;
	}
}

void ModObjAsynchLoader::ParseUseMtl(std::string& buffer)
{
	auto tokens = TextUtils::SpliteWhitespaces(buffer);

	for (auto lib : m_Data->materialLibs)
	{
		auto material = lib->GetByName(tokens[1].c_str());

		if (material != nullptr)
		{
			m_Data->materials.push_back(material);

			mobjmesh_t mesh;

			mesh.material = material;

			m_Data->meshes.push_back(mesh);
			return;
		}

		
	}

}

size_t ModObjAsynchLoader::CurrentResource(StateMachineResource id)
{
	switch (id)
	{
	case StateMachineResource::Object:
		assert(m_Data->objects.size() > 0);
		return m_Data->objects.size() - 1;
	case StateMachineResource::Group:
		assert(m_Data->groups.size() > 0);
		return m_Data->groups.size() - 1;
	case StateMachineResource::Material:
		assert(m_Data->materials.size() > 0);
		return m_Data->materials.size() - 1;
	default:
		assert("Unknown state machine resource!");
		break;

	}
}

ModObjAsynchLoader::ModObjAsynchLoader(ModelOBJ * pModel, const char* fileName)
{
	m_strFileName = fileName;
	m_pModel = pModel;
	m_Data = m_pModel->GetModelData();
}

ModObjAsynchLoader::~ModObjAsynchLoader()
{

}

ITaskStepResult* ModObjAsynchLoader::ExecuteStep(LoaderThread* loaderThread)
{
	if (!m_pFileData)
	{
		InitializeLoader();
	}

	// TODO: check for end symbol
	if (m_FileOffset >= m_pFileData->Length())
	{
		m_Data->flags |= FL_DATA_LOADED;
		return new BuildDrawMeshTask(m_Data, m_pModel);
	}
		
	std::string buffer;

	// Unicode encoding be like - "I am a joke to you?"
	char* p = ((char*)m_pFileData->Data()) + m_FileOffset;

	

	while (*p)
	{
		if (*p == '\n')
		{
			if (buffer.size() > 0)
			{
				ParseCommand(buffer);
				m_nPerformedSteps = m_FileOffset;

				if (m_FileOffset - m_lastReportedOffset > m_ReportRate)
				{
					m_lastReportedOffset = m_FileOffset;
					return new MeshLoadingProgressStep(m_nPerformedSteps, m_nTotalSteps);
				}
			}

			buffer = "";
			p++;
			m_FileOffset++;
			continue;
		}

		buffer += *p++;
		m_FileOffset++;
	}

	if (buffer.size() > 0)
	{
		ParseCommand(buffer);
		m_nPerformedSteps = m_FileOffset;
		
		if (m_FileOffset - m_lastReportedOffset > m_ReportRate)
		{
			m_lastReportedOffset = m_FileOffset;
			return new MeshLoadingProgressStep(m_nPerformedSteps, m_nTotalSteps);
		}
	}

	return new MeshLoadingProgressStep(m_nPerformedSteps, m_nTotalSteps);
}

void ModObjAsynchLoader::InitializeLoader()
{
	m_strDescription = std::format("Loading \"{0}\"", m_strFileName);

	m_pFileData = Application::GetFileSystem()->LoadFile(m_strFileName);
	m_FileOffset = 0;

	m_nPerformedSteps = 0;
	m_nTotalSteps = m_pFileData->Length();

	// add default mesh

	mobjmesh_t mesh = {0};

	auto fs = Application::GetFileSystem();
	auto baseName = fs->BaseName(m_strFileName);
	auto baseDir = fs->BaseDirectoryFromFileName(m_strFileName.c_str());

	AsynchTextureLoadTask* loaderTask = new AsynchTextureLoadTask("Existing lightmaps");

	mesh.diffuse_texture_path = std::format("{0}/{1}.png", baseDir, baseName);
	mesh.diffuse_texture = loaderTask->ScheduleTexture(mesh.diffuse_texture_path.c_str());

	for (int i = 0; i < MAX_LIGHT_STYLES; i++)
	{
		mesh.lightmap_texture_path[i] = std::format("{0}/{1}_lightmap_{2}.png", baseDir, baseName, i);
		mesh.lightmap_texture[i] = loaderTask->ScheduleTexture(mesh.lightmap_texture_path[i].c_str());
	}

	m_Data->meshes.push_back(mesh);
	
	loaderTask->Schedule();

	// add default material

	mobjmaterial_t* mat = new mobjmaterial_t;
	mat->name = "<no material>";
	m_Data->materials.push_back(mat);

	// add default object

	mobjobjects_t obj;
	strncpy_s(obj.name, "<default object>", sizeof(obj.name) - 1);
	m_Data->objects.push_back(obj);

	// add default group

	mobjegroup_t grp;
	strncpy_s(grp.name, "<ungrouped>", sizeof(grp.name) - 1);

	grp.object_id = CurrentResource(StateMachineResource::Object);
	grp.first_face = m_Data->faces.size();

	m_Data->groups.push_back(grp);

	// ����������� �������� ������ � ��������� ����� �������, ����� �� ������� � �������� �����
	// � ������� ������ 100 ������� �� ����� ��������
	m_ReportRate = m_pFileData->Length() / 100;
}

void ModObjAsynchLoader::OnCompletion()
{	
	return;
}

ModObjAsynchLoader::BuildDrawMeshTask::BuildDrawMeshTask(mobjdata_t* data, ModelOBJ* pModel) : ITaskStepResult(TaskStepResultType::FinishedSuccesfully)
{
	m_pModel = pModel;
	m_pModelData = data;
}

void ModObjAsynchLoader::BuildDrawMeshTask::ExecuteOnCompletion()
{
	m_pModel->BuildDrawMesh();
	m_pModel->AddLightsIntoScene();
	
}

ModObjAsynchLoader::MeshLoadingProgressStep::MeshLoadingProgressStep(size_t progress, size_t totalSteps): ITaskStepResult(TaskStepResultType::StepPerformed)
{
	m_nProgress = progress;
	m_nTotalSteps = totalSteps;

	m_strElementDescription = std::format("{0} of {1} bytes parsed", m_nProgress, m_nTotalSteps);
}

ModObjAsynchLoader::MeshLoadingProgressStep::~MeshLoadingProgressStep()
{

}
