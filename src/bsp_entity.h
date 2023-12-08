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

class BSPEntityProperty;

class BSPEntity : public SceneEntity
{
    FGDEntityClass *m_pFGDClass = nullptr;
    
    static glm::vec4 ConvertLightColorAndIntensity(Lb3kLightEntity *pEntity);

    std::shared_ptr<BSPWorld> m_World;

    std::list<BSPEntityProperty *> m_lstProperties;

    GLTexture *m_pEditorSprite = nullptr;

    friend class BSPEntityProperty;

  public:
    // Delete for now, implement later
    BSPEntity(BSPEntity *pOther) = delete;
    BSPEntity(Scene * pScene);
    ~BSPEntity();

    void SetKeyValue(std::string &key, std::string &value);
    void PopulateScene();

    bool UpdateProperties();
    void Export(FILE *fp);

    void RenderBoundingBox() override;
    void RenderGroupShaded() override;
    void RenderLightshaded() override;
    void RenderUnshaded() override;

    void OnSelect() override;

    bool HasProperty(size_t hash);
    std::list<BSPEntityProperty *> &GetBSPProperties();

    void SetFGDClass(FGDEntityClass *pClass);
};

} // namespace GoldSource
