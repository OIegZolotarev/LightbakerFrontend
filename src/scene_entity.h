/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#pragma once
#include "selection_3d.h"

class SceneEntity: public ISelectableObject
{
	bool m_bDataLoaded = false;
protected:
	void FlagDataLoaded()
	{
		m_bDataLoaded = true;
	}
public:
	SceneEntity() {};
	
	virtual void RenderLightshaded() {}; // С лайтмапой
	virtual void RenderUnshaded() {}; // Без лайтмапы
	virtual void RenderBoundingBox() {}; // 
	virtual void RenderDebug() {}; // Отладочная отрисовка


	bool IsDataLoaded()
	{
		return m_bDataLoaded;
	}
};

typedef std::shared_ptr<SceneEntity> SceneEntityPtr;