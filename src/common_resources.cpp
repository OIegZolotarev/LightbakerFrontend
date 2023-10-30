/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "common.h"
#include "common_resources.h"
#include "application.h"

std::unordered_map<CommonIcons, gltexture_t*> *  g_CommonIcons = nullptr;

void LoadIcon(CommonIcons id, const char* filePath)
{
	FileData* pData = Application::GetFileSystem()->LoadFile(filePath);

	std::pair<CommonIcons, gltexture_t*> p(id, LoadGLTexture(pData));
	g_CommonIcons->insert(p);

	delete pData;
}

void DestroyCommonResources()
{
	g_CommonIcons->clear();
}

void InitCommonResources()
{
	g_CommonIcons = new std::unordered_map<CommonIcons, gltexture_t*>;

	LoadIcon(CommonIcons::LoadFile,		"res/loadfile.png");
	LoadIcon(CommonIcons::Bake,			"res/bake.png");
	LoadIcon(CommonIcons::DirectLight,	"res/directlight.png");
	LoadIcon(CommonIcons::OmniLight,	"res/omnilight.png");
	LoadIcon(CommonIcons::SpotLight,	"res/spotlight.png");
}

gltexture_t* GetCommonIcon(CommonIcons icon_id)
{
	if (!g_CommonIcons->count(icon_id))
		return 0;

	return (*g_CommonIcons)[icon_id];	
}
