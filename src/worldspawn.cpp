/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#include "application.h"
#include "worldspawn.h"
#include "properties_editor.h"

Worldspawn::Worldspawn()
{
	SetClassName("worldspawn");
	m_EnvColor = glm::vec3(1, 1, 1);
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


void WorldspawnPropertiesBinder::FillProperties(std::vector<VariantValue>& collection)
{
	auto ptr = m_ptr.lock();

	if (!ptr)
		return;

	VariantValue p;

	p = VariantValue(EnvColor, PropertiesTypes::ColorRGB, "Environment color");
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

