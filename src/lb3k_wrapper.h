#pragma once

#include "common.h"

enum class LightMapTypes
{
	Regular = 0,
	DeluxeTangent,
	DeluxeWorld,
	DeluxeUnity,
	RNM,
	AO,
	BentNormals,
	Curvature
};

enum class LightMapFileFormat
{
	NONE = 0,
	PNG_RGB8,
	PNG_RGB16,
	PNG_RGBA8,
	PNG_RGBA16,
	TGA_RGB,
	TGA_RGBA,
	BMP,
	JPG
};

#define BKF_NO_GI						(1<<0)
#define BKF_16BIT_BUFFER				(1<<1)
#define BKF_NO_SHADOW_TERMINATOR_FIX	(1<<2)

#define BSPF_ENV_DYNLIGHT_GI			(1<<1)
#define BSPF_ENV_STATIC_SHADOW			(1<<2)
#define BSPF_GFX_ENV_SKY_COLOR			(1<<3)
#define BSPF_BAKE_LIGHTPROBES			(1<<4)
#define BSPF_NEW_DWORDLIGHT_FORMAT		(1<<5)

#define PPF_EXP_TONEMAPPING				(1<<6)
#define PPF_DELUXEMAP_DELAMBERT			(1<<7)
#define PPF_LOG_COMPRESS				(1<<8)
#define PPF_CLAMP_LIGHTING				(1<<9)
#define PPF_WHITE_EMISS_MATERIAL		(1<<10)


class LightBaker3000
{
	std::string m_strEXELocation;

	struct 
	{
		uint32_t width = 1024;
		uint32_t height = 1024;

		LightMapTypes lm_type = LightMapTypes::RNM;
		LightMapFileFormat file_format = LightMapFileFormat::PNG_RGB8;
	}m_lmSettings;
		
	int m_iFlags = PPF_DELUXEMAP_DELAMBERT | BSPF_BAKE_LIGHTPROBES | BSPF_ENV_DYNLIGHT_GI | BSPF_ENV_STATIC_SHADOW | BSPF_GFX_ENV_SKY_COLOR;

	float m_flBSPSmootAngle = 30.f;
	int	  m_iPaddingSize = 8;
	int   m_iDenoiserRadius = 4;
	float m_flPPMultiplier = 1;
	int	  m_flDenoiserThreshold = 8;
	float m_flGamma = 2.2f;
	
	int m_iNoiseBlurRadius = 4;
	int m_iNoiseBlurThreshold = 8;
	int m_iBlurRadius = 0;
	

	float m_flSamples = 256;
	float m_flMaxDistAO = 0;

	int m_iSeamsIteration = 0;
	float m_flSeamsAngle = 1;

public:
	LightBaker3000(const std::string& exeLocation);
	~LightBaker3000();

	std::string	BuildOptionsString(bool bObjModel);
};

