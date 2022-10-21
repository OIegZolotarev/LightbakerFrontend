/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#include "imgui_popups.h"

class SceneScaleDialog : public IImGUIPopup
{
	float m_flScale = 0;
	float m_flScaleOriginal = 0;

public:
	SceneScaleDialog();
	~SceneScaleDialog();

	void Render() override;

	void OnOpen() override;

};