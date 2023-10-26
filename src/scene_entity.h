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
	
	virtual void RenderLightshaded() {}; // � ���������
	virtual void RenderUnshaded() {}; // ��� ��������
	virtual void RenderBoundingBox() {}; // 
	virtual void RenderDebug() {}; // ���������� ���������
};

typedef std::shared_ptr<SceneEntity> SceneEntityPtr;