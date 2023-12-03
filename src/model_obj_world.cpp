/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#include "application.h"
#include "model_obj_world.h"
#include "properties_editor.h"
#include "goldsource_bsp_level.h"
#include "camera.h"

ModelObjWorld::ModelObjWorld(const char* fileName)
{
	SetClassName("worldspawn");
	m_EnvColor = glm::vec3(1, 1, 1);

	auto fs = Application::GetFileSystem();
	auto ext = fs->ExtensionFromPath(fileName);

	m_pObjWorld = new ModelOBJ(fileName);

}

ModelObjWorld::~ModelObjWorld()
{
	if (m_pObjWorld) delete m_pObjWorld;
}

void ModelObjWorld::OnSelect()
{
	auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();
	auto scene = sceneRenderer->GetScene();

	auto weakRef = scene->GetEntityWeakRef(this);

	WorldspawnPropertiesBinder* binder = new WorldspawnPropertiesBinder(weakRef);
	LoadPropertiesToPropsEditor(binder);
}


std::string ModelObjWorld::ExportForCompiling(const char* newPath, lightBakerSettings_t* lb3kOptions)
{
	if (m_pObjWorld)
		return m_pObjWorld->Export(newPath,lb3kOptions, m_EnvColor);

	return "none";
}

void ModelObjWorld::RenderBoundingBox()
{

}

void ModelObjWorld::RenderDebug()
{
	
}

void ModelObjWorld::RenderForSelection(int objectId, class SceneRenderer* sc)
{
	if (m_pObjWorld)
		m_pObjWorld->RenderForSelection(objectId, sc);
}

void ModelObjWorld::RenderGroupShaded()
{
	if (m_pObjWorld)
		m_pObjWorld->RenderGroupShaded();
}

void ModelObjWorld::RenderLightshaded()
{
	if (m_pObjWorld)
		m_pObjWorld->RenderLightshaded();
}

void ModelObjWorld::RenderUnshaded()
{
	if (m_pObjWorld)
		m_pObjWorld->RenderUnshaded();
}

bool ModelObjWorld::IsDataLoaded()
{
	if (m_pObjWorld)
		return m_pObjWorld->IsDataLoaded();

	return false;
}

void ModelObjWorld::ReloadLightmaps()
{
	if (m_pObjWorld)
		m_pObjWorld->ReloadLightmapTextures();
}

void ModelObjWorld::OnAdditionToScene()
{	
}

EntityClasses ModelObjWorld::EntityClass()
{
	return EntityClasses::World;
}

void WorldspawnPropertiesBinder::FillProperties(std::vector<VariantValue>& collection)
{
	auto ptr = m_ptr.lock();

	if (!ptr)
		return;

	ModelObjWorld* world = (ModelObjWorld*)ptr.get();

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

	ModelObjWorld* world = (ModelObjWorld*)ptr.get();

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
	return "World (Wavefront Obj)";
}

