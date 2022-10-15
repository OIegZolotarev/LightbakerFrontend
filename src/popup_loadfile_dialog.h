/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once
#include "imgui_popups.h"
#include "lb3k_wrapper.h"

class LoadFileDialog : public IImGUIPopup
{
public:
	LoadFileDialog();
	~LoadFileDialog();

	void Render() override;
};
