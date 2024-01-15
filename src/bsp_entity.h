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
    FGDEntityClassWeakPtr m_pFGDClass;

    static glm::vec4 ConvertLightColorAndIntensity(Lb3kLightEntity *pEntity);

    std::shared_ptr<BSPWorld> m_World;
    
    

    bool m_bIsTransparent = false;

    friend class BSPEntityProperty;

    

public:
    // Delete for now, implement later if necceseary
    BSPEntity(BSPEntity *pOther);
    BSPEntity(Scene *pScene);
    ~BSPEntity();

    void SetKeyValue(const std::string &key, const std::string &value);
    void PopulateScene();

    void Export(FILE *fp);

    void OnSelect(ISelectableObjectWeakRef myWeakRef) override;

    BSPEntityProperty *             FindProperty(size_t prophash);
    bool                            HasProperty(size_t hash);
    std::list<BSPEntityProperty *> &GetBSPProperties();

    void                  SetFGDClass(FGDEntityClassWeakPtr pClass);
    FGDEntityClassWeakPtr GetFGDClass();

    void UpdateProperty(BSPEntityProperty *pNewProperty);

    void Render(RenderMode mode, const SceneRenderer * sr, ShaderProgram *shader) override;
    void UpdatePropertyPosition(BSPEntityProperty *p, glm::vec3 delta);

    SceneEntity *Clone() override;
};

} // namespace GoldSource
