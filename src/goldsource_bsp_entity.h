/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "goldsource_bsp_level.h"
#include "goldsource_bsp_world.h"
#include "hammer_fgd.h"
#include "lights.h"
#include "scene_entity.h"
#include <unordered_map>
#include <utility>

namespace GoldSource
{
class BSPEntity : public SceneEntity
{
    FGDEntityClass *m_pFGDClass = nullptr;

    typedef std::pair<std::string, std::string> kvData;
    std::unordered_map<std::string, std::string> m_vProperties;

    glm::vec3 origin = {0, 0, 0};
    glm::vec4 color  = {0, 0, 0, 0};

    std::weak_ptr<SceneEntity> m_SceneEntity;

    glm::vec3 ConvertOriginToSceneSpace();
    static glm::vec3 ConvertOriginFromSceneSpace(glm::vec3 pos);
    static glm::vec4 ConvertLightColorAndIntensity(Lb3kLightEntity *pEntity);

    std::shared_ptr<BSPWorld> m_World;

  public:
    BSPEntity();
    ~BSPEntity();

    void SetKeyValue(std::string &key, std::string &value);
    void PopulateScene();

    bool UpdateProperties();
    void Export(FILE *fp);
};

} // namespace GoldSource
