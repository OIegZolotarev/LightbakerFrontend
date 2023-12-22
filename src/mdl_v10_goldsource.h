/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "common.h"
#include "mdl_v10_disk_structs_goldsource.h"
#include "text_utils.h"
#include "mod_manager.h"

class FileData;
class DrawMesh;
class IndexedFromMemoryImage;

namespace GoldSource
{
struct StudioEntityState
{
    glm::vec3 origin;
    int       bodynum;
    int       skin;
    int       sequence;
    float     frame;
    float     blending[2];

    float controller[4];
    float mouth;
};

class StudioModelV10;
class StudioSubModelV10;

class StudioBoneV10
{
public:
    struct dofInfo_s
    {
        int   controller = -1;
        float value      = 0;
        float scale      = 0;
    };

private:
    char m_strName[32];
    int  m_iParent = -1;

    int m_iFlags = 0;

    dofInfo_s m_DofInfo[6];

public:
    StudioBoneV10(dstudiobone10_t *pBone);

    int GetParent();

    dofInfo_s *GetDofInfo(size_t index)
    {
        return &m_DofInfo[index];
    }
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
public:
    struct blendInfo_s
    {
        int   type;
        float start;
        float end;
    };

    int   SequenceGroup();
    int   MotionType();
    int   MotionBone();
    int   NumFrames();
    float FPS();

private:
    char  m_strName[32];
    float m_flFps;

    int m_iFlags;

    int m_iActivity;
    int m_iActivityWeight;

    std::vector<StudioPivotV10> m_vPivots;

    int m_iNumFrames;

    std::vector<StudioEventV10> m_vEvents;

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

    blendInfo_s m_Blends[2];
    int         m_iBlendParent;

    int m_iSeqGroup; // sequence group for demand loading

    int m_iEntryNode; // transition node at entry
    int m_iExitNode;  // transition node at exit
    int m_iNodeFlags; // transition rules

    int m_iNextSeq; // auto advancing sequences

public:
    StudioSeqDescV10(byte *header, dstudioseqdesc10_t *pDesc);

    const blendInfo_s *GetBlendInfo(size_t index) const
    {
        assert(index >= 0 && index < 2);
        return &m_Blends[index];
    }

    const int NumBlends() const
    {
        return numblends;
    }

    const int AnimsIndex() const
    {
        return animindex;
    }
};

class StudioSeqGroupV10
{
    char  m_strLabel[32];
    char  m_strName[64];
    void *m_pCache = nullptr;
    void *m_pData;

public:
    StudioSeqGroupV10(byte *hdr, dstudioseqgroup10_t *pGroup);
    dstudioanim10_t *GetAnimation(StudioSeqDescV10 *pSeqDesc);
};

class StudioTextureV10
{
    char m_strName[64];
    int  m_iFlags;
    int  m_iWidth;
    int  m_iHeight;

    IndexedFromMemoryImage *m_pPixels  = nullptr;
    GLTexture *             m_pTexture = nullptr;

    bool m_bFallback = false;

public:
    StudioTextureV10(GLTexture * fallback);

    StudioTextureV10(byte *header, dstudiotexture10_t *pTexture);
    ~StudioTextureV10();
    int Width();
    int Height();

    GLTexture *GetGLTexture();
};

class StudioMeshV10
{
    int numtris;
    int triindex;
    int skinref;

    int numnorms;
    int normindex;

    DrawMesh *m_pDrawMesh = nullptr;

    StudioModelV10 *   m_pModel;
    StudioSubModelV10 *m_pSubmodel;
    void               BuildDrawMesh();

public:
    StudioMeshV10(StudioModelV10 *pModel, StudioSubModelV10 *pSubModel, dstudiomesh10_t *pMesh);
    ~StudioMeshV10();
    void DrawPoints(StudioEntityState *pState);
};

class StudioSubModelV10
{
public:
    struct subModelVert
    {
        glm::vec3 origin;
        byte      bone;

        subModelVert(glm::vec3 &pt, byte _bone)
        {
            origin = pt;
            bone   = _bone;
        }
    };

    struct subModelNorm
    {
        glm::vec3 normal;
        byte      bone;

        subModelNorm(glm::vec3 &pt, byte _bone)
        {
            normal = pt;
            bone   = _bone;
        }
    };

private:
    char m_strName[64];

    std::vector<StudioMeshV10 *> m_vMeshes;

    std::vector<subModelVert> m_vVerts;
    std::vector<subModelNorm> m_vNorms;

    int numnorms;      // number of unique surface normals
    int norminfoindex; // normal bone info
    int normindex;     // normal vec3_t

public:
    StudioSubModelV10(StudioModelV10 *pMainModel, dstudiomodel10_t *pModel);
    ~StudioSubModelV10();

    void DrawPoints(StudioEntityState *pState) const;

    const std::vector<subModelVert> &GetVertices() const
    {
        return m_vVerts;
    }

    const std::vector<subModelNorm> &GetNormals() const
    {
        return m_vNorms;
    }
};

class StudioBodyPartV10
{
    char m_strName[64];
    int  m_iBase;

    std::vector<StudioSubModelV10 *> m_vModels;

public:
    StudioBodyPartV10(StudioModelV10 *pModel, dstudiobodypart10_t *pBodyPart);
    ~StudioBodyPartV10()
    {
        ClearPointersVector(m_vModels);
    }
    const int Base() const;
    const int NumModels() const;

    const StudioSubModelV10 *SubModel(size_t index) const
    {
        return m_vModels[index];
    }
};

class StudioAttachmentV10
{
    char m_strName[32];
    int  m_iType;

    int m_iBone;

    glm::vec3 m_Origin;
    glm::vec3 m_Vectors[3];

public:
    StudioAttachmentV10(dstudioattachment10_t *pAttachment);
};

class StudioBoneControllerV10
{
    int32_t bone; // -1 == 0
    int32_t type; // X, Y, Z, XR, YR, ZR, M

    float start;
    float end;

    int32_t rest;  // byte index value at rest
    int32_t index; // 0-3 user set controller, 4 mouth

public:
    StudioBoneControllerV10(dstudiobonecontroller10_t *pController)
    {
        bone  = pController->bone;
        type  = pController->type;
        start = pController->start;
        end   = pController->end;

        rest  = pController->rest;
        index = pController->index;
    }
    int   Index();
    int   Type();
    float Start();
    float End();
};

class StudioModelV10 : public IModel
{
    FileData *m_pFileData;

    int                m_iNumSkinFamilies;
    std::vector<short> m_vSkins;

    std::vector<StudioBoneV10>           m_vBones;
    std::vector<StudioHitBoxV10>         m_vHitBoxes;
    std::vector<StudioSeqDescV10>        m_vSequences;
    std::vector<StudioSeqGroupV10>       m_vSeqGroups;
    std::vector<StudioTextureV10 *>      m_vTextures;
    std::vector<StudioBodyPartV10 *>     m_vBodyParts;
    std::vector<StudioAttachmentV10>     m_vAttachments;
    std::vector<StudioBoneControllerV10> m_vBoneControllers;

    StudioEntityState *m_EntityState;

    void SetupBones(void);

    dstudioanim10_t *GetAnim(StudioSeqDescV10 *pSeqDesc);

    void CalcBoneAdj();
    void CalcRotations(glm::vec3 *pos, glm::quat *q, StudioSeqDescV10 *pSeqDesc, dstudioanim10_t *pAnim, float f);

    void CalcBoneQuaternion(int frame, float s, StudioBoneV10 *pbone, dstudioanim10_t *panim, glm::quat &q);
    void CalcBonePosition(int frame, float s, StudioBoneV10 *pBone, dstudioanim10_t *pAnim, glm::vec3 &pos);

    void SlerpBones(glm::quat *q1, glm::vec3 *pos1, glm::quat *q2, glm::vec3 *pos2, float s);

    const StudioSubModelV10 *SetupModel(int bodypart) const;

    void OverlayBones();

public:
    StudioModelV10(FileData *fd);
    ~StudioModelV10();

    void DebugRender();

    // Entity state
    int   SetBodygroup(int iGroup, int iValue);
    int   SetSkin(int iValue);
    int   SetSequence(int iSequence);
    float SetBlending(int iBlender, float flValue);

    void AdvanceFrame(float dt);

    byte *            Header();
    short             GetSkinRef(int skinref);
    StudioTextureV10 *GetTexture(short textureIdx);

    void Render(SceneEntity *pEntity, RenderMode mode) override;
};

} // namespace GoldSource