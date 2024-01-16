/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "brush_primitives_pd.h"
#include "prefabs_factory.h"

// Brush primitives id
#define BP_BOX      1
#define BP_CYLINDER 2

BrushPrimitvesDepartament::BrushPrimitvesDepartament() : IPrefabDepartament("Primitives")
{
    prefabDescrpitor_t d;
    d.description         = "Cube";
    d.canTweakDetailLevel = false;
    d.id                  = BP_BOX;

    m_vecItems.push_back(d);
}

const size_t BrushPrimitvesDepartament::AvaibleDesignsCount() const
{
    return m_vecItems.size();
}

const void BrushPrimitvesDepartament::ConstructDesign(size_t designIdx, Scene *pScene, const BoundingBox &dimensions,
                                                      size_t detailLevel) const
{
    const prefabDescrpitor_t *d = &m_vecItems[designIdx];

    switch (d->id)
    {
    case BP_BOX:
        ConstructBox(pScene, dimensions);
        break;
    }
}

const prefabDescrpitor_t *BrushPrimitvesDepartament::DesignDesription(size_t idx) const
{
    return &m_vecItems[idx];
}

void BrushPrimitvesDepartament::ConstructBox(Scene *pScene, const BoundingBox &dimensions)
{
    float e = 0.5f;

    mprimitveface_s boxFaces[6] = {
        // clang-format off
        {{{e  , e  , e} , {e   , e  , -e} , {e  , -e , e}}  , "" , {{0.f  , 1.f  , 0.f , 0.f} , {0.f , 0.f  , -1.f , -1.f}} , 0.f, {1.f, 1.f}},
        {{{-e , -e , e} , {-e  , -e , -e} , {-e , e  , e}}  , "" , {{0.f  , -1.f , 0.f , 0.f} , {0.f , 0.f  , -1.f , -1.f}} , 0.f, {1.f, 1.f}},
        {{{e  , -e , e} , {e   , -e , -e} , {-e , -e , e}}  , "" , {{1.f  , 0.f  , 0.f , 0.f} , {0.f , 0.f  , -1.f , -1.f}} , 0.f, {1.f, 1.f}},
        {{{-e , e  , e} , {-e  , e  , -e} , {e  , e  , e}}  , "" , {{-1.f , 0.f  , 0.f , 0.f} , {0.f , 0.f  , -1.f , -1.f}} , 0.f, {1.f, 1.f}},
        {{{-e , e  ,-e} ,  {-e , -e , -e} , {e  , e  , -e}} , "" , {{-1.f , 0.f  , 0.f , 0.f} , {0.f , -1.f ,  0.f  , 0.f}} , 0.f, {1.f, 1.f}},
        {{{e  , -e , e} , {-e  , -e , e}  , {e  , e  , e}}  , "" , {{1.f  , 0.f  , 0.f , 0.f} , {0.f , -1.f ,  0.f  , 0.f}} , 0.f, {1.f, 1.f}}
        // clang-format on
    };
}

void BrushPrimitvesDepartament::ConstructBox(Scene *pScene, const BoundingBox &dimensions) const
{
}
