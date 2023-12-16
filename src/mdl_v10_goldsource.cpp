/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "mdl_v10_goldsource.h"
#include "file_system.h"

using namespace GoldSource;

StudioBoneV10::StudioBoneV10(dstudiobone10_t *pBone)
{
    strlcpy(m_strName, pBone->name, sizeof(m_strName));

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
    dstudiohdrv10_t *hdr = (dstudiohdrv10_t *)fd->Data();

    if (hdr->version != 10)
        // throw new UnsupportedVersionException
        return;

    m_pFileData = fd;

    // Bones

    dstudiobone10_t *pBones = (dstudiobone10_t *)(fd->Data() + hdr->bones.index);

    m_vBones.reserve(hdr->bones.num_items);

    for (int i = 0; i < hdr->bones.num_items; i++)
        m_vBones.push_back(StudioBoneV10(&pBones[i]));

    // Hitboxes

    dstudiobbox10_t *pHitBoxes = (dstudiobbox10_t *)(fd->Data() + hdr->hitboxes.index);

    m_vHitBoxes.reserve(hdr->hitboxes.num_items);

    for (int i = 0; i < hdr->hitboxes.num_items; i++)
        m_vHitBoxes.push_back(StudioHitBoxV10(&pHitBoxes[i]));

    // Sequences

    dstudioseqdesc10_t *pSequences = (dstudioseqdesc10_t *)(fd->Data() + hdr->sequences.index);

    m_vSequences.reserve(hdr->sequences.num_items);

    for (int i = 0; i < hdr->sequences.num_items; i++)
        m_vSequences.push_back(StudioSeqDescV10(fd->Data(), &pSequences[i]));

    // Sequence groups

    dstudioseqgroup10_t *pSeqGroups = (dstudioseqgroup10_t *)(fd->Data() + hdr->seqgroups.index);

    m_vSeqGroups.reserve(hdr->seqgroups.num_items);

    for (int i = 0; i < hdr->seqgroups.num_items; i++)
        m_vSeqGroups.push_back(StudioSeqGroupV10(&pSeqGroups[i]));

    // Textures

    dstudiotexture10_t *pTexture = (dstudiotexture10_t *)(fd->Data() + hdr->textures.index);

    m_vTextures.reserve(hdr->textures.num_items);

    for (int i = 0; i < hdr->textures.num_items; i++)
        m_vTextures.push_back(StudioTextureV10(fd->Data(),&pTexture[i]));
    
        
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

StudioSeqDescV10::StudioSeqDescV10(byte *header, dstudioseqdesc10_t *pDesc)
{
    strlcpy(m_strName, pDesc->label, sizeof(m_strName));

    m_flFps  = pDesc->fps;
    m_iFlags = pDesc->flags;

    m_iActivity       = pDesc->activity;
    m_iActivityWeight = pDesc->actweight;

    dstudiopivot10_t *pivots = (dstudiopivot10_t *)(header + pDesc->pivotindex);

    for (int i = 0; i < pDesc->numpivots; i++)
    {
        m_vPivots.push_back(StudioPivotV10(&pivots[i]));
    }

    m_iNumFrames = pDesc->numframes;

    m_iMotionType       = pDesc->motiontype;
    m_iMotionBone       = pDesc->motionbone;
    m_vecLinearMovement = pDesc->linearmovement;

    m_iAutoMovePosindex   = pDesc->automoveposindex;
    m_iAutomoveAngleIndex = pDesc->automoveangleindex;

    m_vecMin = pDesc->bbmin;
    m_vecMax = pDesc->bbmax;

    numblends = pDesc->numblends;
    animindex = pDesc->animindex;

    for (int i = 0; i < 2; i++)
    {
        m_Blends[i].start = pDesc->blendstart[i];
        m_Blends[i].end   = pDesc->blendend[i];
        m_Blends[i].type  = pDesc->blendtype[i];
    }

    m_iBlendParent = pDesc->blendparent;
    m_iSeqGroup    = pDesc->seqgroup;

    m_iEntryNode = pDesc->entrynode;
    m_iExitNode  = pDesc->exitnode;
    m_iNodeFlags = pDesc->nodeflags;

    m_iNextSeq = pDesc->nextseq;
}

StudioPivotV10::StudioPivotV10(dstudiopivot10_t *pPivot)
{
    m_vecOrigin = pPivot->org;
    m_iStart    = pPivot->start;
    m_iEnd      = pPivot->end;
}

StudioEventV10::StudioEventV10(dstudioevent10_t *pEvent)
{
    strlcpy(m_strOptions, pEvent->options, sizeof(m_strOptions));

    m_iFrame = pEvent->frame;
    m_iEvent = pEvent->event;
    m_iType  = pEvent->type;
}

StudioBodyPartV10::StudioBodyPartV10(byte *hdr, dstudiobodyparts10_t *pBodyPart)
{
    strlcpy(m_strName, pBodyPart->name, sizeof(m_strName));
    m_iBase = pBodyPart->base;

    dstudiomodel10_t *models = (dstudiomodel10_t *)(hdr + pBodyPart->modelindex);

    m_vModels.reserve(pBodyPart->nummodels);

    for (int i = 0; i < pBodyPart->nummodels; i++)
    {
        m_vModels.push_back(StudioSubModelV10(hdr, &models[i]));
    }
}

StudioMeshV10::StudioMeshV10(dstudiomesh10_t *pMesh)
{
    numtris   = pMesh->numtris;
    triindex  = pMesh->triindex;
    skinref   = pMesh->skinref;
    numnorms  = pMesh->numnorms;
    normindex = pMesh->normindex;
}

StudioSubModelV10::StudioSubModelV10(byte *header, dstudiomodel10_t *pModel)
{
    strlcpy(m_strName, pModel->name, sizeof(m_strName));
    m_iType            = pModel->type;
    m_flBoundingRadius = pModel->boundingradius;

    dstudiomesh10_s *meshes = (dstudiomesh10_t *)header + pModel->meshindex;

    for (int i = 0; i < pModel->nummesh; i++)
    {
        m_vMeshes.push_back(StudioMeshV10(&meshes[i]));
    }
}

StudioTextureV10::StudioTextureV10(byte *header, dstudiotexture10_t *pTexture)
{
    strlcpy(m_strName, pTexture->name, sizeof(m_strName));
    m_iFlags  = pTexture->flags;
    m_iWidth  = pTexture->width;
    m_iHeight = pTexture->height;
}

StudioSeqGroupV10::StudioSeqGroupV10(dstudioseqgroup10_t *pGroup)
{
    strlcpy(m_strLabel, pGroup->label, sizeof(m_strLabel));
    strlcpy(m_strName, pGroup->name, sizeof(m_strName));
    m_iData = pGroup->data;
}
