/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#include "application.h"
#include "worldspawn.h"
#include "properties_editor.h"
#include "goldsource_bsp_world.h"
#include "camera.h"

Worldspawn::Worldspawn(const char* fileName)
{
	SetClassName("worldspawn");
	m_EnvColor = glm::vec3(1, 1, 1);

	auto fs = Application::GetFileSystem();
	auto ext = fs->ExtensionFromPath(fileName);

	if (ext == ".obj")
		m_pObjWorld = new ModelOBJ(fileName);
	else if (ext == ".bsp")
	{		
		auto fd = fs->LoadFile(fileName);
		m_pBSPWorld = new GoldSource::BSPWorld(fd);
		fd->UnRef();

		m_pBSPRenderer = new GoldSource::BSPRenderer(m_pBSPWorld);
	}
}

Worldspawn::~Worldspawn()
{

	if (m_pBSPWorld) delete m_pBSPWorld;
	if (m_pBSPRenderer) delete m_pBSPRenderer;
	if (m_pObjWorld) delete m_pObjWorld;
}

void Worldspawn::OnSelect()
{
	auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();
	auto scene = sceneRenderer->GetScene();

	auto weakRef = scene->GetEntityWeakRef(this);

	WorldspawnPropertiesBinder* binder = new WorldspawnPropertiesBinder(weakRef);
	LoadPropertiesToPropsEditor(binder);
}


std::string Worldspawn::ExportForCompiling(const char* newPath, lightBakerSettings_t* lb3kOptions)
{
	if (m_pObjWorld)
		return m_pObjWorld->Export(newPath,lb3kOptions, m_EnvColor);

	if (m_pBSPWorld)
		return m_pBSPWorld->Export(newPath, lb3kOptions, m_EnvColor);

	return "none";
}

void Worldspawn::RenderBoundingBox()
{

}

void Worldspawn::RenderDebug()
{
	
}

void Worldspawn::RenderForSelection(int objectId, class SceneRenderer* sc)
{
	if (m_pObjWorld)
		m_pObjWorld->RenderForSelection(objectId, sc);
}

void Worldspawn::RenderGroupShaded()
{
	if (m_pObjWorld)
		m_pObjWorld->RenderGroupShaded();

	if (m_pBSPWorld)
	{
		auto cam = Application::GetMainWindow()->GetSceneRenderer()->GetCamera();
		auto pos = cam->GetOrigin();
		m_pBSPRenderer->PerformRendering(pos);
	}
}

void Worldspawn::RenderLightshaded()
{
	if (m_pObjWorld)
		m_pObjWorld->RenderLightshaded();

	if (m_pBSPWorld)
	{
		auto cam = Application::GetMainWindow()->GetSceneRenderer()->GetCamera();
		auto pos = cam->GetOrigin();
		m_pBSPRenderer->PerformRendering(pos);
	}
}

void Worldspawn::RenderUnshaded()
{
	if (m_pObjWorld)
		m_pObjWorld->RenderUnshaded();

	if (m_pBSPWorld)
	{
		auto cam = Application::GetMainWindow()->GetSceneRenderer()->GetCamera();
		auto pos = cam->GetOrigin();
		m_pBSPRenderer->PerformRendering(pos);
	}
}

bool Worldspawn::IsDataLoaded()
{
	if (m_pObjWorld)
		return m_pObjWorld->IsDataLoaded();

	if (m_pBSPWorld)
		return true;

	return false;
}

void Worldspawn::ReloadLightmaps()
{
	if (m_pObjWorld)
		m_pObjWorld->ReloadLightmapTextures();

	if (m_pBSPWorld)
		m_pBSPWorld->ReloadLightmaps();
}

void Worldspawn::OnAdditionToScene()
{
	if (m_pBSPWorld)
		m_pBSPWorld->PopulateScene();
}

EntityClasses Worldspawn::EntityClass()
{
	return EntityClasses::World;
}

void WorldspawnPropertiesBinder::FillProperties(std::vector<VariantValue>& collection)
{
	auto ptr = m_ptr.lock();

	if (!ptr)
		return;

	Worldspawn* world = (Worldspawn*)ptr.get();

	VariantValue p;

	p = VariantValue(EnvColor, PropertiesTypes::ColorRGB, "Environment color");
	p.SetColorRGB(world->GetEnvColor());
	collection.push_back(p);

}

bool WorldspawnPropertiesBinder::IsObjectValid()
{
	return m_ptr.lock() != nullptr;
}

void WorldspawnPropertiesBinder::OnPropertyChangeSavedToHistory()
{
	Application::ScheduleCompilationIfNecceseary();
}

void WorldspawnPropertiesBinder::UpdateObjectProperties(VariantValue* props, size_t num)
{
	auto ptr = m_ptr.lock();
	
	if (!ptr)
		return;

	Worldspawn* world = (Worldspawn*)ptr.get();

	auto history = Application::GetMainWindow()->GetSceneRenderer()->GetScene()->GetEditHistory();

	for (size_t i = 0; i < num; i++)
	{
		auto & it = props[i];

		switch (it.GetId())
		{
		case EnvColor:
			world->SetEnvColor(it.GetColorRGB());
			break;
		}
	}
}

const char* WorldspawnPropertiesBinder::ObjectClassname()
{
	return "World";
}

