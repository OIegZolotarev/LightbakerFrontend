/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/


#define _CRT_SECURE_NO_WARNINGS

#include "camera.h"
#include "model_obj.h"
#include "application.h"
#include "common_resources.h"
#include <intsafe.h>
#include "gl_backend.h"
#include <algorithm>
#include "text_utils.h"

#define WHITE_PNG "res/textures/white.png"
#define DUMMY_PNG "res/textures/dummy.png"

ModelOBJ::ModelOBJ(FileData* pFileData)
{

	m_pMaterialLib = nullptr;
	
	auto baseName = pFileData->BaseName();
	auto baseDir = pFileData->DirName();

	auto mtlName = baseDir + "/" + baseName + ".mtl";
	auto data = Application::GetFileSystem()->LoadFile(mtlName);

	if (data != nullptr)
	{
		m_pMaterialLib = new MaterialTemplateLibrary(data);
		data->UnRef();
	}

	//return;

	// TODO: incapsulate better way?
	
	

	if (m_vMeshes.size() == 0)
	{
		// Load default material...
		std::string modelDir = Application::GetFileSystem()->ParentPath(pFileData->Name());
		std::string baseName = Application::GetFileSystem()->BaseName(pFileData->Name());

		mobjmesh_t defaultMaterial;
		memset(&defaultMaterial, 0, sizeof(defaultMaterial));

		auto diffusePath = modelDir + "/" + baseName + ".png";

		m_strModelName = pFileData->Name();
		m_strDiffuseName = diffusePath;

		FileData* data = 0;

		defaultMaterial.diffuse_texture_path = diffusePath;
		defaultMaterial.diffuse_texture = LoadGLTexture(defaultMaterial.diffuse_texture_path.c_str());

		if (defaultMaterial.diffuse_texture->width == -1)
			defaultMaterial.diffuse_texture = LoadGLTexture(DUMMY_PNG);
		
		for (int i = 0; i < MAX_LIGHT_STYLES; i++)
		{
			auto lmPath = modelDir + "/" + baseName + "_lightmap_" +  std::to_string(i) + ".png";			
			defaultMaterial.lightmap_texture[i] = LoadGLTexture(lmPath.c_str(), true);
			defaultMaterial.lightmap_texture_path[i] = lmPath;

			if (defaultMaterial.lightmap_texture[i]->width == -1)
				defaultMaterial.lightmap_texture[i] = LoadGLTexture(WHITE_PNG);

		}

		m_vMeshes.push_back(defaultMaterial);

		

	}
	

	ParseData(pFileData);

	std::qsort(m_vecFaces.data(), m_vecFaces.size() / 3, sizeof(mobjface_t) * 3, [](const void* pa, const void* pb) -> int
		{
			const mobjface_t* faceA = static_cast<const mobjface_t*>(pa);
			const mobjface_t* faceB = static_cast<const mobjface_t*>(pb);

			if (faceA->mesh_id == faceB->mesh_id)
				return 0;

			else if (faceA->mesh_id > faceB->mesh_id)
				return 1;

			else if (faceA->mesh_id < faceB->mesh_id)
				return -1;

			return 0;
		});

	//BuildDrawMesh();

	if (m_vGroups.size() == 0)
	{

		for (auto& it : m_vecFaces)
			it.group_id = 1;

		mobjgroup_s grp;
		grp.first_face = 0;
		grp.num_faces = m_vMeshes.size() - 1;
		
		strncpy(grp.name, "default", sizeof(grp.name) - 1);

		m_vGroups.push_back(grp);
	}
}

ModelOBJ::~ModelOBJ()
{
	for (auto& it : m_vMeshes)
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
	FreeVector(m_vGroups);

	if (m_pMaterialLib)
		delete m_pMaterialLib;
}

void ModelOBJ::DrawDebug()
{

#if 1
	mesh.Bind();

	glEnable(GL_TEXTURE_2D);

	for (auto& it : m_vMeshes)
	{
		if (it.diffuse_texture)
			glBindTexture(GL_TEXTURE_2D, it.diffuse_texture->gl_texnum);
		else
			glBindTexture(GL_TEXTURE_2D, 0);

		mesh.Draw(it.first_face, it.num_faces);
	}

	mesh.Unbind();
#else
 	if (m_vecFaces.size() == 0)
 		return;
 
 	if (m_vecVertsData.size() == 0)
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
#endif
	
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

	{
		auto tokens = TextUtils::SpliteWhitespaces(buffer);
		assert(m_pMaterialLib);
		mCurrentMaterial = m_pMaterialLib->GetByName(tokens[1].c_str());
			
		AddMaterial(mCurrentMaterial);

	}

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

		if (!TextUtils::EmptyString(token))
		{
			m_vecNormalsData.push_back(stof(token));
			normSize++;
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!TextUtils::EmptyString(token))
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

		if (!TextUtils::EmptyString(token))
		{
			m_vecUVData.push_back(stof(token));
			newUVSize++;
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!TextUtils::EmptyString(token))
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

		if (!TextUtils::EmptyString(token))
		{
			m_vecVertsData.push_back(stof(token));
			newVertSize++;
		}

		s.erase(0, pos + 1);
	} while ((pos = s.find(" ")) != std::string::npos);

	token = s.substr(0, pos);

	if (!TextUtils::EmptyString(token))
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
	auto tokens = TextUtils::SpliteWhitespaces(buffer);

	if (tokens.size() != 2)
	{
		Con_Printf("Malformed group command: %s\n", buffer.c_str());
		return;
	}

	if (m_vGroups.size() > 0)
	{
		mobjegroup_t& grp = m_vGroups[m_vGroups.size() - 1];
		grp.num_faces = m_vMeshes.size() - grp.first_face;
	}

	mobjegroup_t grp;
	strncpy(grp.name, tokens[1].c_str(), sizeof(grp.name) - 1);

	grp.first_face = m_vMeshes.size();
	
	m_vGroups.push_back(grp);

}

void ModelOBJ::ParseFace(std::string& s)
{

	size_t pos = 0; std::string token;

	mobjgroup_s* grp = CurrentGroup();

	bool hasUV = false;
	bool hasNorm = false;

	
	hasUV = m_vecUVData.size() > 0;
	hasNorm = m_vecNormalsData.size() > 0;
	
	

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

		face.mesh_id = mCurrentMeshId;
		face.group_id = m_vGroups.size() - 1;


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
		m_vecFaces.push_back(parsed[0]);
		m_vecFaces.push_back(parsed[i + 1]);
		m_vecFaces.push_back(parsed[i + 2]);
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

		m_LightmapDimensions[0] = stoi(tokens[1]);
		m_LightmapDimensions[1] = stoi(tokens[2]);

		return;
	}

	if (tokens[0] == "#scene_scale")
	{
		if (tokens.size() != 2)
		{
			Con_Printf("[ERROR] Malformed #scene_scale command\n");
			return;
		}

		m_flSceneScale = stof(tokens[1]);
		return;
	}

	if (tokens[0] == "#env_color")
	{
		if (tokens.size() != 4)
		{
			Con_Printf("[ERROR] Malformed #env_color command\n");
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

	auto typeInfo = TextUtils::Split(tokens[0], "_");

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

	newLight.pos[0] = stof(tokens[1]) * m_flSceneScale;
	newLight.pos[1] = stof(tokens[2]) * m_flSceneScale;
	newLight.pos[2] = stof(tokens[3]) * m_flSceneScale;

	newLight.color[0] = stof(tokens[4]);
	newLight.color[1] = stof(tokens[5]);
	newLight.color[2] = stof(tokens[6]);

	newLight.intensity = stof(tokens[7]) * m_flSceneScale;

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
	auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();
	float scale = 1.0f / sceneRenderer->GetSceneScale();

	fprintf(fp, "### LightBaker 3000 lights definitions\n\n");

	fprintf(fp, "#scene_scale %.3f\n\n", sceneRenderer->GetSceneScale());
	
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


		fprintf(fp,"%s\t%.6f %.6f %.6f\t%.6f %.6f %.6f\t%.6f\t%.6f %.3f %.6f\t%.6f %.6f\t%d\t",
				lightBaseType.c_str(),
				it.pos[0] * scale, it.pos[1] * scale, it.pos[2] * scale,
				it.color[0], it.color[1], it.color[2],
				it.intensity * scale,
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
			fprintf(fp, "g %s\n", m_vGroups[lastGroup - 1].name);
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

void ModelOBJ::SetLightmapDimensions(int w, int h)
{
	m_LightmapDimensions[0] = w;
	m_LightmapDimensions[1] = h;
}

void ModelOBJ::BuildDrawMesh()
{
	if (m_vecFaces.size() == 0)
		return;

	if (m_vecVertsData.size() == 0)
		return;

	bool bNoTextures = false;

	size_t currentMaterial = 0;
	mobjmesh_t* pMaterial = nullptr;

	currentMaterial = m_vecFaces[0].mesh_id;
	pMaterial = &m_vMeshes[currentMaterial];

	
	if (pMaterial->lightmap_texture[0])			
		bNoTextures = false;	
	else
		bNoTextures = true;

	
	pMaterial->first_face = mesh.CurrentElement();

	mesh.Begin(GL_TRIANGLES);
	mesh.Color4f(1, 1, 1, 1);

	for (auto& face : m_vecFaces)
	{
		if (face.mesh_id != currentMaterial)
		{
			pMaterial->num_faces = mesh.CurrentElement() - pMaterial->first_face;

			Con_Printf("%d : First face: %d, num faces: %d\n", currentMaterial, pMaterial->first_face, pMaterial->num_faces);

			currentMaterial = face.mesh_id;
			pMaterial = &m_vMeshes[currentMaterial];

			pMaterial->first_face = mesh.CurrentElement();

			assert(pMaterial->material);

			if (pMaterial->lightmap_texture[0])
				bNoTextures = false;
			else
				bNoTextures = true;
		}

		if (m_vecUVData.size() > 0 && face.uv > 0)
		{
			mesh.TexCoord2fv(&m_vecUVData[(face.uv - 1) * m_UVSize]);
		}
		else
		{
			float nullUv[2] = { 0,0 };
			mesh.TexCoord2fv(nullUv);
		}

		if (m_vecNormalsData.size() > 0 && face.norm > 0)
		{
			float* norm = &m_vecNormalsData[(face.norm - 1) * 3];
			mesh.Normal3fv(norm);
		}
		else
		{
			float nullNorm[3] = { 0,0,0 };
			mesh.Normal3fv(nullNorm);
		}

			float * p = &m_vecVertsData[(face.vert - 1) * m_VertSize];
			//Con_Printf("%f %f %f\n", p[0], p[1], p[2]);

			mesh.Vertex3fv(&m_vecVertsData[(face.vert - 1) * m_VertSize]);
		
	}

	pMaterial->num_faces = mesh.CurrentElement() - pMaterial->first_face;

	mesh.End();


}

float ModelOBJ::GetSceneScale()
{
	return m_flSceneScale;
}

void ModelOBJ::AddMaterial(mobjmaterial_t* mat)
{

	for (auto it : m_vMeshes)
	{
		if (it.material == mat)
		{
			mCurrentMeshId = it.id;
			return;
		}
	}

	mobjmesh_s newMesh;
	newMesh.material = mat;
	newMesh.id = m_vMeshes.size();
	mCurrentMeshId = newMesh.id;
	m_vMeshes.push_back(newMesh);	
}

DrawMesh* ModelOBJ::GetDrawMesh()
{
	return &mesh;
}

void ModelOBJ::OnHovered()
{
	
}

void ModelOBJ::OnMouseMove(glm::vec2 delta)
{
	
}

void ModelOBJ::OnSelect()
{
	//Con_Printf("ModelOBJ::OnSelect()\n");
}

void ModelOBJ::OnUnSelect()
{
	
}

void ModelOBJ::OnUnhovered()
{
	
}

void ModelOBJ::RenderForSelection(int objectId, SceneRenderer* pRenderer)
{
	auto sceneReneder = Application::Instance()->GetMainWindow()->GetSceneRenderer();
	auto shader = GLBackend::Instance()->GeometrySelectionShader();
	
	auto identity = glm::mat4(1);

	shader->Bind();

	shader->SetTransform(identity);
	shader->SetScale(sceneReneder->GetSceneScale());
	shader->SetDefaultCamera();
	shader->SetObjectId(objectId);

	mesh.Bind();
	mesh.Draw(0, m_vecFaces.size());	
	
	mesh.Unbind();
	shader->Unbind();
}

void ModelOBJ::RenderBoundingBox()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void ModelOBJ::RenderDebug()
{
	DrawDebug();
}

void ModelOBJ::RenderLightshaded()
{
	auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();

	mesh.Bind();

	auto shader = GLBackend::Instance()->LightMappedSceneShader();
	shader->Bind();
	shader->SetScale(sceneRenderer->GetSceneScale());
	shader->SetDefaultCamera();

	glEnable(GL_TEXTURE_2D);

	//for (int i = 5 ;  i < m_vMeshes.size() - 1; i++)
	for (auto& it : m_vMeshes)
	{
		//auto it = m_vMeshes[i];

		if (it.num_faces == 0)
		{
			continue;
		}

		glActiveTexture(GL_TEXTURE0);
		{
			if (it.material)
			{
				glBindTexture(GL_TEXTURE_2D, it.material->map_Kd->gl_texnum);
			}
			else
			{
				if (it.diffuse_texture)
					glBindTexture(GL_TEXTURE_2D, it.diffuse_texture->gl_texnum);
				else
					glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		glActiveTexture(GL_TEXTURE1);
		{
			if (it.lightmap_texture[0])
				glBindTexture(GL_TEXTURE_2D, it.lightmap_texture[0]->gl_texnum);
			else
				glBindTexture(GL_TEXTURE_2D, 0);
		}

		mesh.Draw(it.first_face, it.num_faces);

		//break;
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);

	shader->Unbind();
	mesh.Unbind();
}

void ModelOBJ::RenderUnshaded()
{
	//throw std::logic_error("The method or operation is not implemented.");
}

void ModelOBJ::ReloadTextures()
{
	for (auto& it : m_vMeshes)
	{
		if (it.diffuse_texture)
			GLReloadTexture(it.diffuse_texture);
		else
		{
			it.diffuse_texture = LoadGLTexture(it.diffuse_texture_path.c_str());
		
			if (it.diffuse_texture->width == -1)
				it.diffuse_texture = LoadGLTexture(DUMMY_PNG);
				
			
		}

		for (int i = 0; i < MAX_LIGHT_STYLES; i++)
		{
			if (it.lightmap_texture[i])
				GLReloadTexture(it.lightmap_texture[i]);
			else
			{
				it.lightmap_texture[i] = LoadGLTexture(it.lightmap_texture_path[i].c_str());

				if (it.lightmap_texture[i]->width == -1)
					it.lightmap_texture[i] = LoadGLTexture(WHITE_PNG);
			}
		}
			
	}
}

void ModelOBJ::Export(const char* fileName)
{
	return;

	FILE* fp = nullptr;
	fp = fopen(fileName, "wt");

	if (!fp)
		Application::EPICFAIL("Error while opening file %s for exporting!", fileName);

	ExportLightDefs(fp);
	ExportVerticles(fp);
	
	if (m_vecUVData.size() > 0)  ExportNormals(fp);
	if (m_vecUVData.size() > 0) ExportUV(fp);

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


