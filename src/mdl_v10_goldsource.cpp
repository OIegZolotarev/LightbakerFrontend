/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "mdl_v10_goldsource.h"

using namespace GoldSource;

StudioBoneV10::StudioBoneV10(dstudiobone10_t *pBone)
{
    strncpy(m_strName, pBone->name, sizeof(m_strName));

    m_iFlags = pBone->flags;

    for (int i = 0; i < 6; i++)
    {
        m_DofInfo[i].controller = pBone->bonecontroller[i];
        m_DofInfo[i].value      = pBone->value[i];
        m_DofInfo[i].scale      = pBone->scale[i];
    }
}

void StudioBoneV10::SetParent(StudioBoneV10 *pBone)
{
    m_pParent = pBone;
}

StudioBoneV10 *StudioBoneV10::GetParent()
{
    return m_pParent;
}

StudioModelV10::StudioModelV10(FileData *fd)
{
}

StudioModelV10::~StudioModelV10()
{
}

StudioHitBoxV10::StudioHitBoxV10(dstudiobbox10_t *pBox)
{
    m_iBone  = pBox->bone;
    m_iGroup = pBox->group;
    m_vecMin = pBox->bbmin;
    m_vecMax = pBox->bbmax;
}
