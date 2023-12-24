/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "goldsource_bsp_world.h"
#include "mod_bsp.h"
#include "mod_mdlv10.h"
#include "text_utils.h"
#include "world_entity.h"

#include "goldsource_bsp_renderer.h"
#include "mod_manager.h"

ModelsManager::~ModelsManager()
{
    m_lstModels.clear();
}

IModelWeakPtr ModelsManager::LookupModel(const char *fileName, bool canFallback)
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

    if (*fileName == '*')
    {
        BSPModelAdapter *pNewModel = new BSPModelAdapter(fileName);

        pNewModel->SetHash(hash);

        IModelSharedPtr pResult = IModelSharedPtr(pNewModel);
        m_lstModels.push_back(pResult);
        return pResult;
    }
    else
    {
        auto ext = FileSystem::ExtractFileExtension(fileName);

        if (ext == "mdl")
        {
            auto fd = FileSystem::Instance()->LoadFile(fileName);

            if (!fd)
            {
                if (!canFallback)
                    return IModelWeakPtr();
                else if (strcasecmp(fileName, "res/mesh/error.mdl"))
                {
                    Con_Printf("ModelsManager::LookupModel(): fallback on %s\n", fileName);
                    return LookupModel("res/mesh/error.mdl", false);
                }
                else
                    return IModelWeakPtr();
            }

            GoldSource::StudioModelV10 *pModel = new GoldSource::StudioModelV10(fd);
            fd->UnRef();

            pModel->SetHash(hash);
            IModelSharedPtr pResult = IModelSharedPtr(pModel);
            m_lstModels.push_back(pResult);
            return pResult;
        }
    }
}

void ModelsManager::OnSceneLoaded(Scene *pScene)
{
    for (auto &it : m_lstModels)
        it->OnSceneLoaded(pScene);
}


void IModel::SetTransparent(bool flag)
{
    m_bTransparent = flag;
}

IModel::IModel(const char *fileName)
{
    m_Hash = StrHash(fileName);
    m_Name = fileName;
}

IModel::~IModel()
{
}

bool IModel::IsTransparent()
{
    return m_bTransparent;
}

size_t IModel::Hash()
{
    return m_Hash;
}

void IModel::SetHash(size_t hash)
{
    m_Hash = hash;
}

const std::optional<BoundingBox> IModel::GetBoundingBox() const
{
    return std::optional<BoundingBox>();
}
