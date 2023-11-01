/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/


#pragma once
#include "persistent.h"

enum class OptionsPage
{
	General = 0,
	Camera,
	Keyboard,
	Total
};

typedef struct uiOptionPage_s
{
	OptionsPage id;

	std::string pageDescription;
	bool selected;
	
	std::vector<VariantValue> properties;
}uiOptionPage_t;

void BeginOptionPage(OptionsPage id, const char* pageDescription);
VariantValue* AddOption(ApplicationSettings setting, const char* description, PropertiesTypes type);