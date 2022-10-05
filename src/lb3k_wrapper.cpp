#include "common.h"
#include "lb3k_wrapper.h"
#include <format>

struct flagsMapping_s
{
	int flag;
	const char* cmdKey;
};

LightBaker3000::LightBaker3000(const std::string& exeLocation)
{
	m_strEXELocation = exeLocation;
}

LightBaker3000::~LightBaker3000()
{

}

std::string LightBaker3000::BuildOptionsString(bool objModel)
{
	std::string result;

	switch (m_Settings.m_lmSettings.lm_type)
	{
	case LightMapTypes::Regular:		
		break;
	case LightMapTypes::DeluxeTangent:
		result += " -deluxe_tangent";
		break;
	case LightMapTypes::DeluxeWorld:
		result += " -deluxe_world";
		break;
	case LightMapTypes::DeluxeUnity:
		result += " -deluxe_unity";
		break;
	case LightMapTypes::RNM:
		result += " -deluxe_rnm";
		break;
	case LightMapTypes::AO:
		result += " -deluxe_ao";
		break;
	case LightMapTypes::BentNormals:
		result += " -bent_normals";
		break;
	case LightMapTypes::Curvature:
		result += " -curvature";
		break;
	default:
		break;

	}

	if (objModel)
	{
		result += std::format(" -width {}", m_Settings.m_lmSettings.size[0]);
		result += std::format(" -height {}", m_Settings.m_lmSettings.size[1]);
	}

	result += std::format(" -samples {}", m_Settings.m_iSamples);
	result += std::format(" -max_dist {}", m_Settings.m_flMaxDistAO);
	result += std::format(" -smooth_angle {}", m_Settings.m_flBSPSmootAngle);

	result += std::format(" -multiplier {}", m_Settings.m_flPPMultiplier);
	result += std::format(" -gamma {}", m_Settings.m_flGamma);
	result += std::format(" -padding {}", m_Settings.m_iPaddingSize);
	result += std::format(" -noise_blur_radius {}", m_Settings.m_iNoiseBlurRadius);
	result += std::format(" -noise_blur_threshold {}", m_Settings.m_iNoiseBlurThreshold);

	result += std::format(" -blur {}", m_Settings.m_iBlurRadius);

	result += std::format(" -seams_iterations {}", m_Settings.m_iSeamsIteration);
	result += std::format(" -seams_angle  {}", m_Settings.m_iSeamsAngle);



	flagsMapping_s flagsMapping[] = 
	{
		{BKF_16BIT_BUFFER,				"-bit16f"},
		{BKF_GI,						"-no_gi"},
		{BKF_SHADOW_TERMINATOR_FIX,		"-no_terminator_fix"},
		{BSPF_GFX_ENV_SKY_COLOR,		"-env_sky"},
		{BSPF_ENV_DYNLIGHT_GI,			"-env_dynlight_gi"},
		{BSPF_ENV_STATIC_SHADOWS,		"-env_static_shadows"},
		{BSPF_BAKE_LIGHTPROBES,			"-lightprobes"},
		{PPF_EXP_TONEMAPPING,			"-tonemapping"},
		{PPF_LOG_COMPRESS,				"-log_compress"},
		{PPF_CLAMP_LIGHTING,			"-clamp"},
		{PPF_DELUXEMAP_DELAMBERT,		"-delambert"},
		{PPF_WHITE_EMISS_MATERIAL,		"-white_emiss"},
		{BSPF_NEW_DWORDLIGHT_FORMAT,	"-new_worldlight"}
	};

	for (auto& it : flagsMapping)
	{
		if (it.flag == BKF_GI)
		{
			if (!(m_Settings.m_iFlags & it.flag))
			{
				result += " ";
				result += it.cmdKey;
			}
		}
		else if (it.flag == BKF_SHADOW_TERMINATOR_FIX)
		{
			if (!(m_Settings.m_iFlags & it.flag))
			{
				result += " ";
				result += it.cmdKey;
			}
		}
		else if (m_Settings.m_iFlags & it.flag)
		{
			result += " ";
			result += it.cmdKey;
		}
				
	}

	return result;
}
