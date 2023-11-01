/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#pragma once

#include "imgui_popups.h"
#include "object_props.h"
#include "ui_options_pages.h"

class OptionsDialog : public IImGUIPopup
{
	float m_flScale = 0;
	float m_flScaleOriginal = 0;

public:
	OptionsDialog();
	~OptionsDialog();

	void Render() override;	
	void OnOpen() override;
private:
	void OnOkPressed();
	void OnCancelPressed();
	void RenderFooter();
	bool RenderHeader();
	
	void RenderOptionsPages(uiOptionPage_t* page);

	void RenderPropertyControl(VariantValue& it);

	void RenderFlagsProperty(VariantValue* it);
	void RenderEnumProperty(VariantValue* it);
	void UpdateProperty(VariantValue* it);
};