#include "common.h"
#include "lb3k_wrapper.h"
#include <format>

LightBaker3000::LightBaker3000(const std::string& exeLocation)
{
	m_strEXELocation = exeLocation;
}

std::string LightBaker3000::BuildOptionsString()
{
	std::string result;

	switch (m_lmSettings.lm_type)
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

	// TODO: check if obj?
	result += std::format(" -width {}",m_lmSettings.width);
	result += std::format(" -height {}", m_lmSettings.height);
	
	result += std::format(" -samples {}", m_flSamples);

	return result;
}
