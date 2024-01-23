/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once


// Matches .map file definitionm, probably should be removed later
typedef struct mprimitveface_s
{
    glm::vec3 planePts[3];
    const char *textureName;
    glm::vec4   uv[2];
    float       rotation;
    glm::vec2   scale;
} mprimitveface_t;

class BrushPrimitvesDepartament : public IPrefabDepartament
{
    std::vector<prefabDescrpitor_t> m_vecItems;

    void ConstructBox(Scene *pScene, const BoundingBox &dimensions) const;

public:
    BrushPrimitvesDepartament();

    const size_t AvaibleDesignsCount() const override;
    const void   ConstructDesign(size_t designIdx, Scene *pScene, const BoundingBox &dimensions,
                                 size_t detailLevel) const override;

    const prefabDescrpitor_t *DesignDesription(size_t idx) const override;
};
