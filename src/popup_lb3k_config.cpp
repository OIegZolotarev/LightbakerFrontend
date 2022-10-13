#include "ui_common.h"
#include "popup_lb3k_config.h"
#include "enum_iterator.h"
#include "lb3k_wrapper.h"
#include "imgui_helpers.h"
#include "application.h"



LB3kConfigPopup::LB3kConfigPopup() : IImGUIPopup(PopupWindows::LightBaker3KConfig)
{

}


LB3kConfigPopup::~LB3kConfigPopup()
{

}

void LB3kConfigPopup::Render()
{
	static const char* key = "LightBaker3000 options";

	if (m_bVisible)
		ImGui::OpenPopup(key);

	if (!ImGui::BeginPopupModal(key, &m_bVisible, ImGuiWindowFlags_AlwaysAutoResize))
		return;

	
	
	RenderSettingsControls(Application::Instance()->GetLightBakerApplication()->Settings());

	if (ImGui::Button("Bake!"))
	{		
		m_bVisible = false;
		Application::Instance()->ExecuteBaking();
	}

	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
	{
		m_bVisible = false;
	}

	ImGui::EndPopup();
}



void LB3kConfigPopup::RenderSettingsControls(lightBakerSettings_t * pSettings)
{
	ImGui::BeginGroup();
	ImGui::Text("Lightmap");

	ImGui::InputInt2("Width & Height", pSettings->m_lmSettings.size);
	
	ImGuiHelpers::EnumCombobox(&pSettings->m_lmSettings.lm_type,"Lightmap type");
	ImGuiHelpers::EnumCombobox(&pSettings->m_lmSettings.file_format, "File format");
		
	ImGui::EndGroup();


	ImGui::BeginGroup();
	ImGui::Text("Baking");

	if (ImGui::BeginTable("split_baking", 2, 0))
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		
		ImGui::CheckboxFlags("Global Illumination",		&pSettings->m_iFlags, BKF_GI);
		ImGui::CheckboxFlags("16bit buffer",			&pSettings->m_iFlags, BKF_16BIT_BUFFER);
		ImGui::CheckboxFlags("Shadow terminator fix",	&pSettings->m_iFlags, BKF_SHADOW_TERMINATOR_FIX);

		ImGui::TableSetColumnIndex(1);

		ImGui::InputInt("Samples", &pSettings->m_iSamples);
		ImGui::InputFloat("AO max dist.", &pSettings->m_flMaxDistAO);

		ImGui::EndTable();
	}
	

	ImGui::EndGroup();


	ImGui::BeginGroup();
	ImGui::Text("BSP");

	if (ImGui::BeginTable("split_bsp", 2, 0))
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);

		ImGui::CheckboxFlags("env_dynlight GI", &pSettings->m_iFlags, BSPF_ENV_DYNLIGHT_GI);
		ImGui::CheckboxFlags("env_static shadows", &pSettings->m_iFlags, BSPF_ENV_STATIC_SHADOWS);
		ImGui::InputFloat("Smooth angle", &pSettings->m_flBSPSmoothAngle);

		ImGui::TableSetColumnIndex(1);

		ImGui::CheckboxFlags("gfx/env sky color", &pSettings->m_iFlags,BSPF_GFX_ENV_SKY_COLOR);
		ImGui::CheckboxFlags("Bake lightprobes", &pSettings->m_iFlags, BSPF_BAKE_LIGHTPROBES);
		ImGui::CheckboxFlags("New dwordlight_t format", &pSettings->m_iFlags, BSPF_NEW_DWORDLIGHT_FORMAT);
		
		ImGui::EndTable();
	}


	ImGui::EndGroup();

	ImGui::BeginGroup();
	ImGui::Text("Post processing");

	if (ImGui::BeginTable("split_post_process", 2, 0))
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);

		ImGui::InputInt("Padding size", &pSettings->m_iPaddingSize);
		ImGui::InputFloat("Multiplier", &pSettings->m_flPPMultiplier);
		ImGui::InputFloat("Gamma", &pSettings->m_flGamma);
		ImGui::InputInt("Blur", &pSettings->m_iBlurRadius);

		ImGui::CheckboxFlags("Deluxemap delambert", &pSettings->m_iFlags, PPF_DELUXEMAP_DELAMBERT);
		ImGui::CheckboxFlags("White emiss. material", &pSettings->m_iFlags, PPF_WHITE_EMISS_MATERIAL);

		ImGui::TableSetColumnIndex(1);

		ImGui::InputInt("Denoiser radius", &pSettings->m_iDenoiserRadius);
		ImGui::InputInt("Denoiser threshold", &pSettings->m_iDenoiserThreshold);

		ImGui::CheckboxFlags("Exp. tonemapping", &pSettings->m_iFlags, PPF_EXP_TONEMAPPING);
		ImGui::CheckboxFlags("Log. compress", &pSettings->m_iFlags, PPF_LOG_COMPRESS);
		ImGui::CheckboxFlags("Clamp lightning", &pSettings->m_iFlags, PPF_CLAMP_LIGHTING);

		ImGui::EndTable();
	}


	ImGui::EndGroup();


	ImGui::BeginGroup();
	ImGui::Text("Seams fix");

	if (ImGui::BeginTable("split_seams_fix", 2, 0))
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);

		ImGui::InputInt("Iterations", &pSettings->m_iSeamsIteration);

		ImGui::TableSetColumnIndex(1);

		ImGui::InputInt("Angle threshold", &pSettings->m_iSeamsAngle);
		

		ImGui::EndTable();
	}


	ImGui::EndGroup();
}

