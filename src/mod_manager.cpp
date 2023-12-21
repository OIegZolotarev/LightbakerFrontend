/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "mod_manager.h"
#include "fs_core.h"
#include "mdl_v10_goldsource.h"
#include "text_utils.h"

ModelsManager::~ModelsManager()
{
    m_lstModels.clear();
}

IModelWeakPtr ModelsManager::LookupModel(const char *fileName)
{
    size_t hash = StrHash(fileName);

    for (auto &it : m_lstModels)
    {
        if (it->Hash() == hash)
        {
            return it;
        }
    }

    // Load new model here

    auto fd = FileSystem::Instance()->LoadFile(fileName);

    if (!fd && strcasecmp(fileName, "res/mesh/error.mdl"))
    {
        Con_Printf("ModelsManager::LookupModel(): fallback on %s\n", fileName);
        return LookupModel("res/mesh/error.mdl");
    }
    else if (!fd)
        return IModelWeakPtr();

    GoldSource::StudioModelV10 *pModel = new GoldSource::StudioModelV10(fd);
    fd->UnRef();

    IModelSharedPtr pResult = IModelSharedPtr(pModel);

    m_lstModels.push_back(pResult);
   
    return pResult;
}

IModel::IModel(const char *fileName)
{
    m_Hash = StrHash(fileName);
}

size_t IModel::Hash()
{
    return m_Hash;
}
