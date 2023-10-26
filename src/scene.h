/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#pragma once

#include "common.h"
#include "lb3k_wrapper.h"
#include "serial_counter.h"
#include "scene_entity.h"

class Scene
{
	lightBakerSettings_s m_lightBakerParams;
	SerialCounter<unsigned int> m_ObjectsCounter;

	std::list<SceneEntityPtr> m_vecSceneLightDefs;

public:
	Scene();
	~Scene();


};