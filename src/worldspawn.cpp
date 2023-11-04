/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#include "application.h"
#include "worldspawn.h"
#include "properties_editor.h"

Worldspawn::Worldspawn(const char* fileName)
{
	SetClassName("worldspawn");
	m_EnvColor = glm::vec3(1, 1, 1);

	auto fs = Application::GetFileSystem();
	auto ext = fs->ExtensionFromPath(fileName);

	if (ext == ".obj")
		m_pObjWorld = new ModelOBJ(fileName);
}

Worldspawn::~Worldspawn()
{

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
}

void Worldspawn::RenderLightshaded()
{
	if (m_pObjWorld)
		m_pObjWorld->RenderLightshaded();
}

void Worldspawn::RenderUnshaded()
{
	if (m_pObjWorld)
		m_pObjWorld->RenderUnshaded();
}

bool Worldspawn::IsDataLoaded()
{
	if (m_pObjWorld)
		return m_pObjWorld->IsDataLoaded();

	return false;
}

void Worldspawn::ReloadLightmaps()
{
	if (m_pObjWorld)
		m_pObjWorld->ReloadLightmapTextures();
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

