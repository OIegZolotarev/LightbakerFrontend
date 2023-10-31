/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#include "application.h"
#include "scene.h"
#include "properties_editor.h"
#include "mod_obj_atlas_gen.h"
#include "mod_obj_asynch_exporter.h"

LevelFormat Scene::DetermineLevelFormatFromFileName(std::string levelName)
{
	auto fs = Application::GetFileSystem();
	auto ext = fs->ExtensionFromPath(levelName);

	if (ext == ".obj")
		return LevelFormat::WavefrontOBJ;
	else if (ext == ".bsp")
		return LevelFormat::BSP;

	return LevelFormat::Unknown;
}

Scene::Scene(const char* levelName, int loadFlags)
{
	m_pEditHistory = new CEditHistory;
	
	if (levelName)
		m_LevelFormat = DetermineLevelFormatFromFileName(levelName);

	if (!(loadFlags & LRF_KEEP_ENTITIES))
		ClearEntities();

	LoadLevel(levelName);
}

Scene::~Scene()
{
	//FreeVector(m_vecSceneLightDefs);
	m_SceneEntities.clear();
	delete m_pEditHistory;
}

void Scene::DeleteEntity(SceneEntityWeakPtr l)
{
	auto ptr = l.lock();

	if (!ptr)
		return;

	auto predicate = [&](SceneEntityPtr& it) -> bool
	{
		return it == ptr;
	};

	auto pos = std::remove_if(m_SceneEntities.begin(), m_SceneEntities.end(), predicate);	
	m_SceneEntities.remove(*pos);		
}

void Scene::DoDeleteSelection()
{
	auto sel = GetSelection();
	auto ptr = sel.lock();

	if (!ptr)
		return;

	m_pEditHistory->PushAction(new CDeleteLightAction(ptr));

	DeleteEntity(sel);
	Application::ScheduleCompilationIfNecceseary();
}

void Scene::AddNewLight(glm::vec3 pos, LightTypes type)
{
	auto newLight = std::make_shared<lightDef_s>();


	// newLight->pos = m_pCamera->GetOrigin() + m_pCamera->GetForwardVector() * 10.f;
	newLight->SetPosition(pos);
	newLight->SetType(type);
	newLight->intensity = 10;

	float hue = (float)rand() / 32768.f;

	glm::vec3 rgb;

	ImGui::ColorConvertHSVtoRGB(hue, 1, 1,
		rgb[0],
		rgb[1],
		rgb[2]);

	newLight->SetColor(rgb);
	newLight->SetSerialNumber(m_ObjectsCounter.Allocate());

	m_SceneEntities.push_back(newLight);
	newLight->InvokeSelect();
}

std::list<SceneEntityPtr>& Scene::GetSceneObjects()
{
	return m_SceneEntities;
}

CEditHistory* Scene::GetEditHistory() const
{
	return m_pEditHistory;
}

void Scene::HintSelected(SceneEntityWeakPtr weakRef)
{
	m_pCurrentSelection = weakRef;
}

SceneEntityWeakPtr Scene::GetSelection()
{
	return m_pCurrentSelection;	
}

void Scene::RenderObjectsFor3DSelection()
{
	auto selectionManager = SelectionManager::Instance();

	for (SceneEntityPtr & it : m_SceneEntities)
	{
		selectionManager->PushObject(it);
	}
}

void Scene::RenderLightShaded()
{
	auto selectionManager = SelectionManager::Instance();

	for (auto & it : m_SceneEntities)
	{
		if (!it)
			continue;

		if (!it->IsDataLoaded())
			continue;

		it->RenderLightshaded();
		selectionManager->PushObject(it);
	}
}

std::list<SceneEntityPtr>& Scene::GetLightDefs()
{
	return m_SceneEntities;
}

bool Scene::IsModelLoaded()
{
	if (m_SceneEntities.size() < 1)
		return false;

	auto it = m_SceneEntities.begin();
	return it->get()->IsDataLoaded();
}

std::string Scene::GetModelFileName()
{
	assert("not implemented yet");
	return "";
}

std::string Scene::GetModelTextureName()
{
	assert("not implemented yet");
	return "";
}

void Scene::AddEntityWithSerialNumber(SceneEntityPtr it, size_t sn)
{
	it->SetSerialNumber(sn);
	m_SceneEntities.push_back(it);
}

SceneEntityWeakPtr Scene::GetEntityBySerialNumber(size_t serialNumber)
{
	for (auto& it : m_SceneEntities)
	{
		if (it->GetSerialNumber() == serialNumber)
			return SceneEntityWeakPtr(it);
	}

	return SceneEntityWeakPtr();
}

void Scene::DeleteEntityWithSerialNumber(size_t serialNumber)
{
	auto lightPtr = GetEntityBySerialNumber(serialNumber);
	DeleteEntity(lightPtr);
}

float Scene::GetSceneScale()
{
	return m_flSceneScale;
}

void Scene::SetScale(float f)
{
	m_flSceneScale = f;
}

void Scene::RescaleLightPositions(float m_flScaleOriginal, float m_flScale)
{
	for (auto& it : m_SceneEntities)
	{
		glm::vec3 pos = it->GetPosition();

		pos /= m_flScaleOriginal;
		pos *= m_flScale;

		it->SetPosition(pos);
	}

	ObjectPropertiesEditor::Instance()->UnloadObject();

	// Not really necessary
	//Application::ScheduleCompilationIfNecceseary();
}

SceneEntityWeakPtr Scene::GetEntityWeakRef(SceneEntity* pEntity)
{
	for (auto & ptr : m_SceneEntities)
	{
		if (ptr.get() == pEntity)
			return SceneEntityWeakPtr(ptr);
	}

	return SceneEntityWeakPtr();
}

void Scene::RenderUnshaded()
{
	auto selectionManager = SelectionManager::Instance();

	for (auto& it : m_SceneEntities)
	{
		if (!it)
			continue;

		if (!it->IsDataLoaded())
			continue;

		it->RenderUnshaded();
		selectionManager->PushObject(it);
	}
}

void Scene::Reload(int loadFlags)
{
	if (loadFlags & LRF_RELOAD_LIGHTMAPS)
	{
		auto it = m_SceneEntities.begin();
		SceneEntity* entity = (*it).get();
		auto obj = (ModelOBJ*)entity;
		obj->ReloadTextures();
	}
}

void Scene::ClearEntities()
{
	if (m_SceneEntities.empty())
		return;

	auto begin = m_SceneEntities.begin(); begin++;
	auto end = m_SceneEntities.end();

	m_SceneEntities.erase(begin, end);
}

void Scene::LoadLevel(const char* levelName)
{
	SceneEntityPtr pLevelEntity = nullptr;

	switch (m_LevelFormat)
	{
	case LevelFormat::Unknown:
		break;
	case LevelFormat::WavefrontOBJ:
		pLevelEntity = std::make_shared<ModelOBJ>(levelName);
		break;
	case LevelFormat::BSP:
		break;
	default:
		break;

	}

	if (!m_SceneEntities.empty())
	{
		auto it = m_SceneEntities.begin(); it++;
		*it = pLevelEntity;
	}
	else
		m_SceneEntities.push_back(pLevelEntity);

}

std::string Scene::ExportForCompiling(const char* newPath)
{
	// TODO: fixme
	auto it = m_SceneEntities.begin();
	SceneEntity* entity = (*it).get();
	auto obj = (ModelOBJ*)entity;
	return obj->Export(newPath);

}

void Scene::RenderGroupsShaded()
{
	auto selectionManager = SelectionManager::Instance();

	for (auto& it : m_SceneEntities)
	{
		if (!it)
			continue;

		if (!it->IsDataLoaded())
			continue;

		it->RenderGroupShaded();
		selectionManager->PushObject(it);
	}
}

void Scene::DumpLightmapMesh()
{
	// TODO: fixme
	auto it = m_SceneEntities.begin();
	SceneEntity* entity = (*it).get();
	auto obj = (ModelOBJ*)entity;

	GenerateAtlasTask* tsk = new GenerateAtlasTask(obj);
	tsk->TransferNewUV();

	ModObjAsynchExporter* exporter = new ModObjAsynchExporter(obj, "lm_debug.obj", true);
	exporter->ExportSynch();
	delete exporter;

}

void Scene::DumpLightmapUV()
{
	// TODO: fixme
	auto it = m_SceneEntities.begin();
	SceneEntity* entity = (*it).get();
	auto obj = (ModelOBJ*)entity;

	GenerateAtlasTask* tsk = new GenerateAtlasTask(obj);
	tsk->DumpUVImage("lm_debug%02u.tga");
	delete tsk;
}
