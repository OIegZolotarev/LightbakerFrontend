/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "common.h"
#include "mdl_v10_disk_structs_goldsource.h"
class FileData;

namespace GoldSource
{
class StudioBoneV10
{
    char            m_strName[32];
    StudioBoneV10 *m_pParent = nullptr;

    int m_iFlags = 0;

    struct dofInfo_s
    {
        int   controller = -1;
        float value      = 0;
        float scale      = 0;
    };

    dofInfo_s m_DofInfo[6];

public:
    StudioBoneV10(dstudiobone10_t *pBone);
    
    void SetParent(StudioBoneV10 *pBone);
    StudioBoneV10 *GetParent();
};

class StudioHitBoxV10
{
    int       m_iBone;
    int       m_iGroup;
    glm::vec3 m_vecMin;
    glm::vec3 m_vecMax;
public:
    StudioHitBoxV10(dstudiobbox10_t *pBox)
    {
        m_iBone = pBox->bone;
        m_iGroup = pBox->group;
        m_vecMin = pBox->bbmin;
        m_vecMax = pBox->bbmax;
    }
};

class StudioModelV10
{
    std::vector<StudioBoneV10> m_vBones;
    std::vector<StudioHitBoxV10> m_vHitBoxes;

public:
    StudioModelV10(FileData *fd);
    ~StudioModelV10();
};

} // namespace GoldSource