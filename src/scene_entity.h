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
	
	virtual void RenderLightshaded() {}; // � ���������
	virtual void RenderUnshaded() {}; // ��� ��������
	virtual void RenderBoundingBox() {}; // 
	virtual void RenderDebug() {}; // ���������� ���������


	bool IsDataLoaded()
	{
		return m_bDataLoaded;
	}
};

typedef std::shared_ptr<SceneEntity> SceneEntityPtr;