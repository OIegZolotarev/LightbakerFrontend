/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once

#include "helpers.h"

class Scene;

typedef struct prefabDescrpitor_s
{
    const char *description         = nullptr;
    bool        canTweakDetailLevel = false;
    size_t      maxDetailLevel      = 0;
    size_t      minDetailLevel      = 0;
} prefabDescrpitor_t;

class IPrefabDepartament
{
    std::string m_strName;

public:
    IPrefabDepartament(const char *name);

    virtual ~IPrefabDepartament();;

    virtual const size_t                    AvaibleDesignsCount() const = 0;
    virtual const prefabDescrpitor_t *DesignDesription(size_t idx) const = 0;

    virtual const void ConstructDesign(size_t designIdx, Scene *pScene, const BoundingBox &dimensions, size_t detailLevel) const = 0;
};

class PrefabsFactory: public Singleton<PrefabsFactory>
{
    friend class Singleton;

    std::vector<IPrefabDepartament *> m_lstDepartaments;

public:
    PrefabsFactory();
    ~PrefabsFactory();

    const IPrefabDepartament *GetPrefabDepartament(size_t idx) const;
};