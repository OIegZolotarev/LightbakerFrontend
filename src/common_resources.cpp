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

	// clang-format off

	LoadIcon(CommonIcons::LoadFile,		"res/ui/icons/loadfile.png");
	LoadIcon(CommonIcons::Bake,			"res/ui/icons/bake.png");
	LoadIcon(CommonIcons::DirectLight,	"res/ui/icons/directlight.png");
	LoadIcon(CommonIcons::OmniLight,	"res/ui/icons/omnilight.png");
	LoadIcon(CommonIcons::SpotLight,	"res/ui/icons/spotlight.png");
	
	
	LoadIcon(CommonIcons::ListAdd      , "res/ui/common_icons/lists/add.png");
    LoadIcon(CommonIcons::ListRemove   , "res/ui/common_icons/lists/remove.png");
    LoadIcon(CommonIcons::ListMoveUp   , "res/ui/common_icons/lists/move_up.png");
    LoadIcon(CommonIcons::ListMoveDown , "res/ui/common_icons/lists/move_down.png");
    LoadIcon(CommonIcons::ListSortDesc , "res/ui/common_icons/lists/sort_desc.png");
    LoadIcon(CommonIcons::ListSortAsc  , "res/ui/common_icons/lists/sort_asc.png");

	// clang-format on
}

gltexture_t* GetCommonIcon(CommonIcons icon_id)
{
	if (!g_CommonIcons->count(icon_id))
		return 0;

	return (*g_CommonIcons)[icon_id];	
}
