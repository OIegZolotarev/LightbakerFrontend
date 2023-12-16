/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "common.h"
#include "mdl_v10_disk_structs_goldsource.h"
#include "text_utils.h"

class FileData;

namespace GoldSource
{

class StudioBoneV10
{
    char           m_strName[32];
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

    void           SetParent(StudioBoneV10 *pBone);
    StudioBoneV10 *GetParent();
};

class StudioHitBoxV10
{
    int       m_iBone;
    int       m_iGroup;
    glm::vec3 m_vecMin;
    glm::vec3 m_vecMax;

public:
    StudioHitBoxV10(dstudiobbox10_t *pBox);
};

class StudioEventV10
{
    int  m_iFrame;
    int  m_iEvent;
    int  m_iType;
    char m_strOptions[64];
public:
    StudioEventV10(dstudioevent10_t *pEvent);    
};

class StudioPivotV10
{
    glm::vec3 m_vecOrigin; // pivot point
    int       m_iStart;
    int       m_iEnd;
public:
    StudioPivotV10(dstudiopivot10_t *pPivot);
};

class StudioSeqDescV10
{
    char  m_strName[32];
    float m_flFps;

    int m_iFlags;

    int m_iActivity;
    int m_iActivityWeight;

    std::vector<StudioEventV10> m_vPivots;

    int m_iNumFrames;

    std::vector<StudioPivotV10> m_vPivots;

    int       m_iMotionType;
    int       m_iMotionBone;
    glm::vec3 m_vecLinearMovement;
    int       m_iAutoMovePosindex;
    int       m_iAutomoveAngleIndex;

    glm::vec3 m_vecMin; // per sequence bounding box
    glm::vec3 m_vecMax;

    int numblends;
    int animindex; // mstudioanim_t pointer relative to start of sequence group data
                   // [blend][bone][X, Y, Z, XR, YR, ZR]

    struct blendInfo_s
    {
        int type;
        float start;
        float end;
    };

    blendInfo_s m_Blends[2];
    int   m_iBlendParent;

    int m_iSeqGroup; // sequence group for demand loading

    int m_iEntryNode; // transition node at entry
    int m_iExitNode;  // transition node at exit
    int m_iNodeFlags; // transition rules

    int m_iNextSeq; // auto advancing sequences

public:
    StudioSeqDescV10(byte*header, dstudioseqdesc10_t *pDesc);
};

class StudioSeqGroupV10
{
    char m_strLabel[32]; 
    char m_strName[64];
    void* m_pCache = nullptr;     
    int  m_iData;      

public:
    StudioSeqGroupV10(dstudioseqgroup10_t * pGroup);
};

class StudioTextureV10
{
    char m_strName[64];
    int  m_iFlags;    
    int  m_iWidth;
    int  m_iHeight;    
public:
    StudioTextureV10(byte* header, dstudiotexture10_t * pTexture);
};

class StudioSubModelV10
{
    char m_strName[64];
    int m_iType;
    float m_flBoundingRadius;

    std::vector<StudioMeshV10> m_vMeshes;

  
    int numverts;      // number of unique vertices
    int vertinfoindex; // vertex bone info
    int vertindex;     // vertex vec3_t

    int numnorms;      // number of unique surface normals
    int norminfoindex; // normal bone info
    int normindex;     // normal vec3_t

    int numgroups; // deformation groups
    int groupindex;

public:
    StudioSubModelV10(byte* header,dstudiomodel10_t *pModel);
};

class StudioMeshV10
{
    int numtris;
    int triindex;
    int skinref;

    int numnorms; 
    int normindex;

public:
    StudioMeshV10(dstudiomesh10_t * pMesh);    
};

class StudioBodyPartV10
{
    char m_strName[64];    
    int  m_iBase;
    
    std::vector<StudioSubModelV10> m_vModels;
public:
    StudioBodyPartV10(byte * hdr, dstudiobodyparts10_t * pBodyPart);
};


class StudioModelV10
{
    FileData * m_pFileData;

    std::vector<StudioBoneV10>   m_vBones;
    std::vector<StudioHitBoxV10> m_vHitBoxes;
    std::vector<StudioSeqDescV10> m_vSequences;
    std::vector<StudioSeqGroupV10> m_vSeqGroups;
    std::vector<StudioTextureV10>  m_vTextures;    
    
    std::vector<StudioBodyPartV10> m_vBodyParts;
           

public:
    StudioModelV10(FileData *fd);
    ~StudioModelV10();
};

} // namespace GoldSource