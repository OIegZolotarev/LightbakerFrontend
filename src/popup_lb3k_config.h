#pragma once
#include "imgui_popups.h"
#include "lb3k_wrapper.h"

class LB3kConfigPopup: public IImGUIPopup
{
public:
	LB3kConfigPopup();
	~LB3kConfigPopup();

	void Render() override;

	void RenderSettingsControls(lightBakerSettings_t * pSettings);

};
