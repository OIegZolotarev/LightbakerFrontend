#include "common.h"
#include "lb3k_wrapper.h"
#include <format>
#include "application.h"
#include "../tiny-process-library/process.hpp"

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
	result += std::format(" -smooth_angle {}", m_Settings.m_flBSPSmoothAngle);

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

int BakeThread(void* args)
{
	TinyProcessLib::Process process1a((char*)args, "", [](const char* bytes, size_t n)
		{
			char* dup = (char*)malloc(sizeof(char) * (n + 1));
			memcpy(dup, bytes, n);
			dup[n] = 0;

			// Make progress output a bit nicer
			for (size_t i = 0; i < (n - 1); i++)
			{
				if (dup[i] == '\r' && dup[i + 1] != '\n')
					dup[i] = '\n';
			}

			Con_Printf(dup);
			// Наверное довольно плохое решение =/ - 30 миллисекунд будет не всегда хватать (на более мощном процессоре\GPU чем у меня);
			SDL_Delay(30);
			free(dup);
		});

	int code = process1a.get_exit_status();
	Application::Instance()->NotifyBakingFinished(code);

	return code;
}

void LightBaker3000::ExecuteBaking(const char* modelFileName)
{
	auto fs = Application::GetFileSystem();
	
	// 1) Скопировать exe в каталог модели?
	
	std::string modelDirectory = fs->BaseDirectoryFromFileName(modelFileName);
	std::string lb3kPath = modelDirectory + "/LightBaker3000.exe";

	if (!fs->FileExists(lb3kPath))
	{
		fs->CopyFile("lb3k/LightBaker3000.exe", lb3kPath.c_str());
	}

	// 2) Сформировать строку запуска

	auto parametersString = BuildOptionsString(false);

	// 3) Запустить поток
	const char* cmdTemplate = "%s %s %s %s";

	auto s = std::string(modelFileName);
	auto baseFileName = fs->BaseName(s) + ".obj";
	const char* diffuseName = "sample.png";

	static char cmd[4096];
	sprintf_s(cmd, cmdTemplate,lb3kPath.c_str(),baseFileName.c_str(),diffuseName,parametersString.c_str());

	Con_Printf("[FRONTEND] Baking started\n");
	Con_Printf("[FRONTEND] cmd: %s\n", cmd);

	SDL_Thread* threadID = SDL_CreateThread(BakeThread, "LightBaker3k thread", (void*)cmd);

}

nlohmann::json lightBakerSettings_s::ToJSON()
{
	nlohmann::json result;
	nlohmann::json lmState;

	lmState["Width"] = m_lmSettings.size[0];
	lmState["Height"] = m_lmSettings.size[1];

	lmState["Type"] = m_lmSettings.lm_type._value;
	lmState["FileFormat"] = m_lmSettings.file_format._value;

	result["LightMap"] = lmState;

	result["Flags"] = m_iFlags;
	result["BSPSmoothAngle"] = m_flBSPSmoothAngle;
	result["PaddingSize"] = m_iPaddingSize;
	result["DenoiserRadius"] = m_iDenoiserRadius;
	result["PPMultiplier"] = m_flPPMultiplier;
	result["DenoiserThreshold"] = m_iDenoiserThreshold;
	result["SeamsAngle"] = m_iSeamsAngle;
	result["SeamsIteration"] = m_iSeamsIteration;
	result["MaxDistAO"] = m_flMaxDistAO;
	result["Samples"] = m_iSamples;
	result["BlurRadius"] = m_iBlurRadius;
	result["NoiseBlurThreshold"] = m_iNoiseBlurThreshold;
	result["NoiseBlurRadius"] = m_iNoiseBlurRadius;
	result["Gamma"] = m_flGamma;

	return result;
}

void lightBakerSettings_s::FromJSON(nlohmann::json& json)
{	
	if (json.contains("LightMap"))
	{
		nlohmann::json lmState = json["LightMap"];

		m_lmSettings.size[0] = lmState["Width"];
		m_lmSettings.size[1] = lmState["Height"];
		
		m_lmSettings.lm_type._value = lmState["Type"];
		m_lmSettings.file_format._value = lmState["FileFormat"];
	}

	m_iFlags				= json["Flags"];
	m_flBSPSmoothAngle		= json["BSPSmoothAngle"];
	m_iPaddingSize			= json["PaddingSize"];
	m_iDenoiserRadius		= json["DenoiserRadius"];
	m_flPPMultiplier		= json["PPMultiplier"];
	m_iDenoiserThreshold	= json["DenoiserThreshold"];
	m_iSeamsAngle			= json["SeamsAngle"];
	m_iSeamsIteration		= json["SeamsIteration"];
	m_flMaxDistAO			= json["MaxDistAO"];
	m_iSamples				= json["Samples"];
	m_iBlurRadius			= json["BlurRadius"];
	m_iNoiseBlurThreshold	= json["NoiseBlurThreshold"];
	m_iNoiseBlurRadius		= json["NoiseBlurRadius"];
	m_flGamma				= json["Gamma"];

}
