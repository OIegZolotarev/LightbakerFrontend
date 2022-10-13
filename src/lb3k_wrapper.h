#pragma once

#include "common.h"
#include "..\include\better-enums\enum.h"
#include <nlohmann/json.hpp>

BETTER_ENUM(LightMapTypes, int, Regular = 1, DeluxeTangent, DeluxeWorld, DeluxeUnity, RNM, AO, BentNormals, Curvature)
BETTER_ENUM(LightMapFileFormat, int, PNG_RGB8,\
	PNG_RGB16,\
	PNG_RGBA8,\
	PNG_RGBA16,\
	TGA_RGB,\
	TGA_RGBA,\
	BMP,\
	JPG);


#define BKF_GI							(1<<0)
#define BKF_16BIT_BUFFER				(1<<1)
#define BKF_SHADOW_TERMINATOR_FIX		(1<<2)

#define BSPF_ENV_DYNLIGHT_GI			(1<<2)
#define BSPF_ENV_STATIC_SHADOWS			(1<<3)
#define BSPF_GFX_ENV_SKY_COLOR			(1<<4)
#define BSPF_BAKE_LIGHTPROBES			(1<<5)
#define BSPF_NEW_DWORDLIGHT_FORMAT		(1<<6)

#define PPF_EXP_TONEMAPPING				(1<<7)
#define PPF_DELUXEMAP_DELAMBERT			(1<<8)
#define PPF_LOG_COMPRESS				(1<<9)
#define PPF_CLAMP_LIGHTING				(1<<10)
#define PPF_WHITE_EMISS_MATERIAL		(1<<11)

typedef struct lightBakerSettings_s
{
	struct
	{
		int32_t size[2] = {1024,1024};
	
		LightMapTypes lm_type = LightMapTypes::RNM;
		LightMapFileFormat file_format = LightMapFileFormat::PNG_RGB8;
	}m_lmSettings;

	int m_iFlags = PPF_DELUXEMAP_DELAMBERT | BSPF_BAKE_LIGHTPROBES | BSPF_ENV_DYNLIGHT_GI | BSPF_ENV_STATIC_SHADOWS | BSPF_GFX_ENV_SKY_COLOR;

	float m_flBSPSmoothAngle = 30.f;
	int	  m_iPaddingSize = 8;
	int   m_iDenoiserRadius = 4;
	float m_flPPMultiplier = 1;
	int	  m_iDenoiserThreshold = 8;
	float m_flGamma = 2.2f;

	int m_iNoiseBlurRadius = 4;
	int m_iNoiseBlurThreshold = 8;
	int m_iBlurRadius = 0;


	int	 m_iSamples = 256;
	float m_flMaxDistAO = 0;

	int m_iSeamsIteration = 0;
	int m_iSeamsAngle = 1;

	nlohmann::json ToJSON();
	void FromJSON(nlohmann::json& json);

}lightBakerSettings_t;

class LightBaker3000
{
	std::string m_strEXELocation;

	lightBakerSettings_t m_Settings;

public:
	LightBaker3000(const std::string& exeLocation);
	~LightBaker3000();

	std::string	BuildOptionsString(bool bObjModel);	

	lightBakerSettings_t* Settings()
	{
		return &m_Settings;
	}

	void ExecuteBaking(const char* modelFileName);
};

