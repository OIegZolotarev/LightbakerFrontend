/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "hammer_fgd.h"
#include <unordered_map>
#include <utility>

namespace GoldSource
{
class BSPEntity
{
    FGDEntityClass *m_pFGDClass = nullptr;

    typedef std::pair<std::string, std::string> kvData;
    std::unordered_map<std::string, std::string> m_vProperties;

    glm::vec3 origin = {0, 0, 0};
    glm::vec4 color  = {0, 0, 0, 0};

    std::weak_ptr<SceneEntity> m_SceneEntity;

    glm::vec3 ConvertOriginToSceneSpace();
    static glm::vec3 ConvertOriginFromSceneSpace(glm::vec3 pos);
    static glm::vec4 ConvertLightColorAndIntensity(LightEntity *pEntity);

  public:

    BSPEntity();
    ~BSPEntity();

    void SetKeyValue(std::string &key, std::string &value);
    void PopulateScene();

    bool UpdateProperties();
    void Export(FILE *fp);
};

} // namespace GoldSource
