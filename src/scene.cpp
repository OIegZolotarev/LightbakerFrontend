/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#include "application.h"
#include "scene.h"
#include "properties_editor.h"

Scene::Scene()
{
	m_pEditHistory = new CEditHistory;
}

Scene::~Scene()
{
	//FreeVector(m_vecSceneLightDefs);
	m_vecSceneLightDefs.clear();
	delete m_pEditHistory;
}

void Scene::DeleteEntity(SceneEntityWeakPtr l)
{
	auto ptr = l.lock();

	if (!ptr)
		return;

	auto pos = std::remove_if(m_vecSceneLightDefs.begin(), m_vecSceneLightDefs.end(), [&](SceneEntityPtr& it)
		{
			return it == ptr;
		});


	m_vecSceneLightDefs.erase(pos);
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
	newLight->type = type;
	newLight->UpdateEditorIcon();
	newLight->intensity = 10;

	float hue = (float)rand() / 32768.f;

	float rgb[3];

	ImGui::ColorConvertHSVtoRGB(hue, 1, 1,
		rgb[0],
		rgb[1],
		rgb[2]);

	newLight->serial_number = m_ObjectsCounter.Allocate();

	m_vecSceneLightDefs.push_back(newLight);
	newLight->InvokeSelect();
}

std::list<SceneEntityPtr>& Scene::GetSceneObjects()
{
	return m_vecSceneLightDefs;
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

	if (m_pSceneModel)
	{
		selectionManager->PushObject(m_pSceneModel);
	}

	for (SceneEntityPtr & it : m_vecSceneLightDefs)
	{
		selectionManager->PushObject(it);
	}
}

void Scene::RenderLightShaded()
{
	auto selectionManager = SelectionManager::Instance();

	if (m_pSceneModel && m_pSceneModel->IsDataLoaded())
	{
		m_pSceneModel->RenderLightshaded();
		selectionManager->PushObject(m_pSceneModel);
	}
}

std::list<SceneEntityPtr>& Scene::GetLightDefs()
{
	return m_vecSceneLightDefs;
}

bool Scene::IsModelLoaded()
{
	if (!m_pSceneModel)
		return false;

	return m_pSceneModel->IsDataLoaded();
}

std::string Scene::GetModelFileName()
{
	if (!m_pSceneModel)
		return "";

	return m_pSceneModel->GetModelFileName();
}

std::string Scene::GetModelTextureName()
{
	if (!m_pSceneModel)
		return "";

	return m_pSceneModel->GetModelTextureName();
}

void Scene::AddEntityWithSerialNumber(SceneEntityPtr it, size_t sn)
{
	it->SetSerialNumber(sn);
	m_vecSceneLightDefs.push_back(it);
}

SceneEntityWeakPtr Scene::GetEntityBySerialNumber(size_t serialNumber)
{
	for (auto& it : m_vecSceneLightDefs)
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
	for (auto& it : m_vecSceneLightDefs)
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
	for (auto & ptr : m_vecSceneLightDefs)
	{
		if (ptr.get() == pEntity)
			return SceneEntityWeakPtr(ptr);
	}

	return SceneEntityWeakPtr();
}
