/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#pragma once
#include "selection_3d.h"

class SceneEntity: public ISelectableObject
{
public:
	SceneEntity() {};
	
	virtual void RenderLightshaded() {}; // С лайтмапой
	virtual void RenderUnshaded() {}; // Без лайтмапы
	virtual void RenderBoundingBox() {}; // 
	virtual void RenderDebug() {}; // Отладочная отрисовка
};

typedef std::shared_ptr<SceneEntity> SceneEntityPtr;