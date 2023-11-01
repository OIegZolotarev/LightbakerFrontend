#include "application.h"
#include "common.h"
#include "ui_options_pages.h"

uiOptionPage_t g_OptionsPages[(int)OptionsPage::Total];
uiOptionPage_t* builderCurrentPage = nullptr;

void BeginOptionPage(OptionsPage id, const char* pageDescription)
{
	builderCurrentPage = &g_OptionsPages[(int)id];
	builderCurrentPage->pageDescription = pageDescription;
	builderCurrentPage->properties.clear();
	builderCurrentPage->pageDescription = pageDescription;
}


VariantValue* AddOption(ApplicationSettings setting, const char* description, PropertiesTypes type)
{
	VariantValue data((int)setting, type, description);
	builderCurrentPage->properties.push_back(data);

	return &builderCurrentPage->properties[builderCurrentPage->properties.size() - 1];
}

