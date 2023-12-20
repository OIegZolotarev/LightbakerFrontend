/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "gl_backend.h"
#include "img_indexed_from_memory.h"
#include "mathlib.h"
#include "mdl_v10_goldsource.h"

using namespace GoldSource;

struct
{
    glm::vec4   boneAdj;
    glm::mat4x4 boneTransform[MAXSTUDIOBONES];
} g_StudioRenderState;

StudioBoneV10::StudioBoneV10(dstudiobone10_t *pBone)
{
    strlcpy(m_strName, pBone->name, sizeof(m_strName));

    m_iFlags  = pBone->flags;
    m_iParent = pBone->parent;

    for (int i = 0; i < 6; i++)
    {
        m_DofInfo[i].controller = pBone->bonecontroller[i];
        m_DofInfo[i].value      = pBone->value[i];
        m_DofInfo[i].scale      = pBone->scale[i];
    }
}

int StudioBoneV10::GetParent()
{
    return m_iParent;
}

StudioModelV10::StudioModelV10(FileData *fd) : IModel(fd->Name().c_str())
{
    dstudiohdrv10_t *hdr = (dstudiohdrv10_t *)fd->Data();

    if (hdr->version != 10)
        // throw new UnsupportedVersionException
        return;

    m_pFileData = fd;

    // Skins

    m_iNumSkinFamilies = hdr->num_skinref;

    short *skinRefs = (short *)(fd->Data() + hdr->skins_index);
    m_vSkins.reserve(hdr->num_skinref);
    for (int i = 0; i < hdr->num_skinref; i++)
    {
        m_vSkins.push_back(skinRefs[i]);
    }

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
        m_vSeqGroups.push_back(StudioSeqGroupV10(fd->Data(), &pSeqGroups[i]));

    // Textures

    dstudiotexture10_t *pTexture = (dstudiotexture10_t *)(fd->Data() + hdr->textures.index);

    m_vTextures.reserve(hdr->textures.num_items);

    for (int i = 0; i < hdr->textures.num_items; i++)
        m_vTextures.push_back(new StudioTextureV10(fd->Data(), &pTexture[i]));

    // Bodyparts

    dstudiobodypart10_t *pBodyPart = (dstudiobodypart10_t *)(fd->Data() + hdr->bodyparts.index);

    m_vBodyParts.reserve(hdr->bodyparts.num_items);

    for (int i = 0; i < hdr->bodyparts.num_items; i++)
    {
        m_vBodyParts.push_back(new StudioBodyPartV10(this, &pBodyPart[i]));
    }

    // Attachments

    dstudioattachment10_t *pAttachments = (dstudioattachment10_t *)(fd->Data() + hdr->attachments.index);

    m_vAttachments.reserve(hdr->attachments.num_items);

    for (int i = 0; i < hdr->attachments.num_items; i++)
    {
        m_vAttachments.push_back(StudioAttachmentV10(&pAttachments[i]));
    }

    fd->Ref();
}

StudioModelV10::~StudioModelV10()
{
    ClearPointersVector(m_vTextures);
    ClearPointersVector(m_vBodyParts);
}

StudioEntityState state;

void StudioModelV10::DebugRender()
{
    
    //memset(&state, 0, sizeof(state));

    
    m_EntityState = &state;
    
    AdvanceFrame(Application::GetMainWindow()->FrameDelta());
    glCullFace(GL_FRONT);

    SetupBones();

    
    
    for (int i = 0; i < (int)m_vBodyParts.size(); i++)
    {
        const StudioSubModelV10 *subModel = SetupModel(i);
        subModel->DrawPoints(m_EntityState);
    }

    // OverlayBones();

    glCullFace(GL_BACK);
}

int StudioModelV10::SetBodygroup(int iGroup, int iValue)
{
    if (iGroup > m_vBodyParts.size())
        return -1;

    StudioBodyPartV10 *bodyPart = m_vBodyParts[iGroup];

    int bpBase      = bodyPart->Base();
    int bpNumModels = bodyPart->NumModels();

    int iCurrent = (m_EntityState->bodynum / bpBase) % bpNumModels;

    if (iValue >= bodyPart->NumModels())
        return iCurrent;

    m_EntityState->bodynum = (m_EntityState->bodynum - (iCurrent * bpBase) + (iValue * bpBase));

    return iValue;
}

int StudioModelV10::SetSkin(int iValue)
{
    if (iValue < m_iNumSkinFamilies)
    {
        return m_EntityState->skin;
    }

    m_EntityState->skin;

    return iValue;
}

int StudioModelV10::SetSequence(int iSequence)
{
    if (iSequence >= m_vSequences.size())
        iSequence = 0;

    if (iSequence < 0)
        iSequence = (int)m_vSequences.size() - iSequence;

    m_EntityState->sequence = iSequence;
    m_EntityState->frame    = 0;

    return iSequence;
}

float StudioModelV10::SetBlending(int iBlender, float flValue)
{
    StudioSeqDescV10 *                   pSeqDesc = &m_vSequences[m_EntityState->sequence];
    const StudioSeqDescV10::blendInfo_s *info     = pSeqDesc->GetBlendInfo(iBlender);

    if (info->type == 0)
        return flValue;

    if (info->type & (STUDIO_XR | STUDIO_YR | STUDIO_ZR))
    {
        // ugly hack, invert value if end < start
        if (info->end < info->start)
            flValue = -flValue;

        // does the controller not wrap?
        if (info->start + 359.0 >= info->end)
        {
            if (flValue > ((info->start + info->end) / 2.0) + 180)
                flValue = flValue - 360;
            if (flValue < ((info->start + info->end) / 2.0) - 180)
                flValue = flValue + 360;
        }
    }

    int setting = 255 * (flValue - info->start) / (info->end - info->start);

    if (setting < 0)
        setting = 0;
    if (setting > 255)
        setting = 255;

    m_EntityState->blending[iBlender] = setting;

    return setting * (1.0 / 255.0) * (info->end - info->start) + info->start;
}

void R_ConcatTransforms(const glm::mat4x4 &in1, const glm::mat4x4 &in2, glm::mat4x4 &out)
{
    out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] + in1[0][2] * in2[2][0];
    out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] + in1[0][2] * in2[2][1];
    out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] + in1[0][2] * in2[2][2];
    out[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] + in1[0][2] * in2[2][3] + in1[0][3];
    out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] + in1[1][2] * in2[2][0];
    out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] + in1[1][2] * in2[2][1];
    out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] + in1[1][2] * in2[2][2];
    out[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] + in1[1][2] * in2[2][3] + in1[1][3];
    out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] + in1[2][2] * in2[2][0];
    out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] + in1[2][2] * in2[2][1];
    out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] + in1[2][2] * in2[2][2];
    out[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] + in1[2][2] * in2[2][3] + in1[2][3];
}

void StudioModelV10::SetupBones(void)
{
    static glm::vec3 pos[MAXSTUDIOBONES];
    static glm::quat q[MAXSTUDIOBONES];

    static glm::vec3 pos2[MAXSTUDIOBONES];
    static glm::quat q2[MAXSTUDIOBONES];
    static glm::vec3 pos3[MAXSTUDIOBONES];
    static glm::quat q3[MAXSTUDIOBONES];
    static glm::vec3 pos4[MAXSTUDIOBONES];
    static glm::quat q4[MAXSTUDIOBONES];

    StudioSeqDescV10 *pSeqDesc = &m_vSequences[m_EntityState->sequence];
    dstudioanim10_t * pAnim    = GetAnim(pSeqDesc);

    CalcRotations(pos, q, pSeqDesc, pAnim, m_EntityState->frame);

    if (pSeqDesc->NumBlends() > 1)
    {
        float  s;
        size_t nBones = m_vBones.size();

        pAnim += nBones;

        CalcRotations(pos2, q2, pSeqDesc, pAnim, m_EntityState->frame);
        s = m_EntityState->blending[0] / 255.0;

        SlerpBones(q, pos, q2, pos2, s);

        if (pSeqDesc->NumBlends() == 4)
        {
            pAnim += nBones;
            CalcRotations(pos3, q3, pSeqDesc, pAnim, m_EntityState->frame);

            pAnim += nBones;
            CalcRotations(pos4, q4, pSeqDesc, pAnim, m_EntityState->frame);

            s = m_EntityState->blending[0] / 255.0;
            SlerpBones(q3, pos3, q4, pos4, s);

            s = m_EntityState->blending[1] / 255.0;
            SlerpBones(q, pos, q3, pos3, s);
        }
    }

    for (size_t i = 0; i < m_vBones.size(); i++)
    {
        glm::mat4x4 boneMatrix = glm::toMat4(q[i]);

        boneMatrix = glm::transpose(boneMatrix);

        boneMatrix[0][3] = pos[i][0];
        boneMatrix[1][3] = pos[i][1];
        boneMatrix[2][3] = pos[i][2];

        int parentBone = m_vBones[i].GetParent();

        if (parentBone == -1)
        {
            g_StudioRenderState.boneTransform[i] = boneMatrix;
        }
        else if (parentBone >= 0)
        {
            // TODO: figure out difference between R_ConcatTransforms and matrix multiplication
            /* g_StudioRenderState.boneTransform[i] = g_StudioRenderState.boneTransform[parentBone] * boneMatrix;*/
            R_ConcatTransforms(g_StudioRenderState.boneTransform[parentBone], boneMatrix,
                               g_StudioRenderState.boneTransform[i]);
        }
    }
}

GoldSource::dstudioanim10_t *StudioModelV10::GetAnim(StudioSeqDescV10 *pSeqDesc)
{
    StudioSeqGroupV10 *pGroup = &m_vSeqGroups[pSeqDesc->SequenceGroup()];
    return pGroup->GetAnimation(pSeqDesc);
}

void StudioModelV10::CalcBoneAdj()
{
    float value;

    int j = 0;

    for (auto &it : m_vBoneControllers)
    {
        int i = it.Index();

        int   type       = it.Type();
        float controller = m_EntityState->controller[i];

        if (i <= 3)
        {
            // check for 360% wrapping
            if (type & STUDIO_RLOOP)
            {
                value = controller * (360.0 / 256.0) + it.Start();
            }
            else
            {
                value = controller / 255.0;
                value = std::clamp(value, 0.f, 1.f);
                value = (1.0f - value) * it.Start() + value * it.End();
            }
        }
        else
        {
            value = m_EntityState->mouth / 64.0;
            if (value > 1.0)
                value = 1.0;
            value = (1.0 - value) * it.Start() + value * it.End();
        }

        switch (it.Type() & STUDIO_TYPES)
        {
        case STUDIO_XR:
        case STUDIO_YR:
        case STUDIO_ZR:
            g_StudioRenderState.boneAdj[j] = value * (glm::pi<float>() / 180.0f);
            break;
        case STUDIO_X:
        case STUDIO_Y:
        case STUDIO_Z:
            g_StudioRenderState.boneAdj[j] = value;
            break;
        }

        j++;
    }
}

void GoldSource::StudioModelV10::CalcRotations(glm::vec3 *pos, glm::quat *q, StudioSeqDescV10 *pSeqDesc,
                                               dstudioanim10_t *pAnim, float f)
{
    int   frame;
    float s;

    frame = (int)f;
    s     = (f - frame);

    // add in programatic controllers
    CalcBoneAdj();

    for (size_t i = 0; i < m_vBones.size(); i++)
    {
        CalcBoneQuaternion(frame, s, &m_vBones[i], &pAnim[i], q[i]);
        CalcBonePosition(frame, s, &m_vBones[i], &pAnim[i], pos[i]);
    }

    int motionType = pSeqDesc->MotionType();
    int motionBone = pSeqDesc->MotionBone();

    if (motionType & STUDIO_X)
        pos[motionBone][0] = 0.0;
    if (motionType & STUDIO_Y)
        pos[motionBone][1] = 0.0;
    if (motionType & STUDIO_Z)
        pos[motionBone][2] = 0.0;
}

void StudioModelV10::CalcBoneQuaternion(int frame, float s, StudioBoneV10 *pBone, dstudioanim10_t *pAnim, glm::quat &q)
{
    int                   j, k;
    glm::quat             q1, q2;
    glm::vec3             angle1, angle2;
    dstudioanimvalue10_t *pAnimValue;

    for (j = 0; j < 3; j++)
    {
        auto dofInfo = pBone->GetDofInfo(j + 3);

        if (pAnim->offset[j + 3] == 0)
        {
            angle2[j] = angle1[j] = dofInfo->value; // default;
        }
        else
        {
            pAnimValue = (dstudioanimvalue10_t *)((byte *)pAnim + pAnim->offset[j + 3]);
            k          = frame;

            while (pAnimValue->num.total <= k)
            {
                k -= pAnimValue->num.total;
                pAnimValue += pAnimValue->num.valid + 1;
            }
            // Bah, missing blend!
            if (pAnimValue->num.valid > k)
            {
                angle1[j] = pAnimValue[k + 1].value;

                if (pAnimValue->num.valid > k + 1)
                {
                    angle2[j] = pAnimValue[k + 2].value;
                }
                else
                {
                    if (pAnimValue->num.total > k + 1)
                        angle2[j] = angle1[j];
                    else
                        angle2[j] = pAnimValue[pAnimValue->num.valid + 2].value;
                }
            }
            else
            {
                angle1[j] = pAnimValue[pAnimValue->num.valid].value;
                if (pAnimValue->num.total > k + 1)
                {
                    angle2[j] = angle1[j];
                }
                else
                {
                    angle2[j] = pAnimValue[pAnimValue->num.valid + 2].value;
                }
            }

            angle1[j] = dofInfo->value + angle1[j] * dofInfo->scale;
            angle2[j] = dofInfo->value + angle2[j] * dofInfo->scale;
        }

        if (dofInfo->controller != -1)
        {
            angle1[j] += g_StudioRenderState.boneAdj[dofInfo->controller];
            angle2[j] += g_StudioRenderState.boneAdj[dofInfo->controller];
        }
    }

    if (angle1 != angle2)
    {
        q1 = glm::quat(angle1);
        q2 = glm::quat(angle2);

        q = glm::slerp(q1, q2, s);
    }
    else
    {
        q = glm::quat(angle1);
    }
}

byte *StudioModelV10::Header()
{
    return m_pFileData->Data();
}

const StudioSubModelV10 *GoldSource::StudioModelV10::SetupModel(int bodypart) const
{
    int index;

    if (bodypart > m_vBodyParts.size() - 1)
    {
        Con_Printf("StudioModelV10::SetupModel: no such bodypart %d\n", bodypart);
        bodypart = 0;
    }

    auto &bodyPart = m_vBodyParts[bodypart];

    index = m_EntityState->bodynum / bodyPart->Base();
    index = index % bodyPart->NumModels();

    return bodyPart->SubModel(index);
}

void StudioModelV10::Render(SceneEntity *pEntity, RenderMode mode)
{
    m_EntityState         = &state;
    m_EntityState->origin = pEntity->GetPosition();

    DebugRender();
}

void StudioModelV10::AdvanceFrame(float dt)
{
    StudioSeqDescV10 &seq = m_vSequences[m_EntityState->sequence];

    if (dt > 0.1)
        dt = (float)0.1;
    m_EntityState->frame += dt * seq.FPS();

    int nFrames = seq.NumFrames();

    if (nFrames <= 1)
    {
        m_EntityState->frame = 0;
    }
    else
    {
        if (m_EntityState->frame < 0)
        {
            m_EntityState->frame = nFrames - 1;
        }
        else
        {
            
            m_EntityState->frame -= (int)(m_EntityState->frame / (nFrames - 1)) * (nFrames - 1);
        }
    }
}

void StudioModelV10::OverlayBones()
{
    auto &bt = g_StudioRenderState.boneTransform;

    for (size_t i = 0; i < m_vBones.size(); i++)
    {
        glm::vec3 bp = {bt[i][0][3], bt[i][1][3], bt[i][2][3]};

        int parent = m_vBones[i].GetParent();

        static DrawMesh mesh(DrawMeshFlags::Dynamic);

        auto shader = GLBackend::Instance()->SolidColorGeometryShader();
        shader->Bind();

        for (auto &it : shader->Uniforms())
        {
            switch (it->Kind())
            {
            case UniformKind::Color:
                it->SetFloat4({1, 1, 1, 1});
                break;
            case UniformKind::TransformMatrix:
                it->SetMat4(glm::mat4x4(1.f));
                break;
            case UniformKind::ObjectSerialNumber:
                it->SetInt(1);
                break;
            default:
                GLBackend::SetUniformValue(it);
                break;
            }
        }

        if (m_vBones[i].GetParent() != -1)
        {
            glPointSize(3.0f);

            mesh.Begin(GL_LINES);

            shader->UniformByKind(UniformKind::Color)->SetFloat4({1, 0.7f, 0, 1});
            // mesh.Color3f(1, 0.7f, 0);

            mesh.Vertex3f(bt[parent][0][3], bt[parent][1][3], bt[parent][2][3]);
            mesh.Vertex3f(bt[i][0][3], bt[i][1][3], bt[i][2][3]);
            mesh.End();

            mesh.BindAndDraw();

            shader->UniformByKind(UniformKind::Color)->SetFloat4({0, 0, 0.8f, 1});

            // glColor3f(0, 0, 0.8f);

            mesh.Begin(GL_POINTS);
            if (m_vBones[parent].GetParent() != -1)
                mesh.Vertex3f(bt[parent][0][3], bt[parent][1][3], bt[parent][2][3]);

            mesh.Vertex3f(bt[i][0][3], bt[i][1][3], bt[i][2][3]);
            mesh.End();
            mesh.BindAndDraw();
        }
        else
        {
            // draw parent bone node
            glPointSize(5.0f);

            // glColor3f(0.8f, 0, 0);
            shader->UniformByKind(UniformKind::Color)->SetFloat4({0.8f, 0, 0, 1});

            mesh.Begin(GL_POINTS);
            mesh.Vertex3f(bt[i][0][3], bt[i][1][3], bt[i][2][3]);
            mesh.End();
            mesh.BindAndDraw();
        }
    }

    glPointSize(1.0f);
}

GoldSource::StudioTextureV10 *StudioModelV10::GetTexture(short textureIdx)
{
    // Out of bounds should be handled by stl lib
    // assert(textureIdx >= 0 && textureIdx < m_vTextures.size());
    return m_vTextures[textureIdx];
}

short StudioModelV10::GetSkinRef(int skinref)
{
    if (skinref >= m_vSkins.size())
        return 0;

    return m_vSkins[skinref];
}

void StudioModelV10::SlerpBones(glm::quat *q1, glm::vec3 *pos1, glm::quat *q2, glm::vec3 *pos2, float s)
{
    int       i;
    glm::quat q3;
    float     s1;

    if (s < 0)
        s = 0;
    else if (s > 1.0)
        s = 1.0;

    s1 = 1.0 - s;

    for (i = 0; i < m_vBones.size(); i++)
    {
        q3 = glm::slerp(q1[i], q2[i], s);

        q1[i][0]   = q3[0];
        q1[i][1]   = q3[1];
        q1[i][2]   = q3[2];
        q1[i][3]   = q3[3];
        pos1[i][0] = pos1[i][0] * s1 + pos2[i][0] * s;
        pos1[i][1] = pos1[i][1] * s1 + pos2[i][1] * s;
        pos1[i][2] = pos1[i][2] * s1 + pos2[i][2] * s;
    }
}

void StudioModelV10::CalcBonePosition(int frame, float s, StudioBoneV10 *pBone, dstudioanim10_t *pAnim, glm::vec3 &pos)
{
    int                   j, k;
    dstudioanimvalue10_t *pAnimValue;

    for (j = 0; j < 3; j++)
    {
        StudioBoneV10::dofInfo_s *dofInfo = pBone->GetDofInfo(j);

        pos[j] = dofInfo->value; // default;

        if (pAnim->offset[j] != 0)
        {
            pAnimValue = (dstudioanimvalue10_t *)((byte *)pAnim + pAnim->offset[j]);

            k = frame;

            // find span of values that includes the frame we want
            while (pAnimValue->num.total <= k)
            {
                k -= pAnimValue->num.total;
                pAnimValue += pAnimValue->num.valid + 1;
            }
            // if we're inside the span
            if (pAnimValue->num.valid > k)
            {
                // and there's more data in the span
                if (pAnimValue->num.valid > k + 1)
                {
                    pos[j] += (pAnimValue[k + 1].value * (1.0 - s) + s * pAnimValue[k + 2].value) * dofInfo->scale;
                }
                else
                {
                    pos[j] += pAnimValue[k + 1].value * dofInfo->scale;
                }
            }
            else
            {
                // are we at the end of the repeating values section and there's another section with data?
                if (pAnimValue->num.total <= k + 1)
                {
                    pos[j] += (pAnimValue[pAnimValue->num.valid].value * (1.0 - s) +
                               s * pAnimValue[pAnimValue->num.valid + 2].value) *
                              dofInfo->scale;
                }
                else
                {
                    pos[j] += pAnimValue[pAnimValue->num.valid].value * dofInfo->scale;
                }
            }
        }
        if (dofInfo->controller != -1)
        {
            pos[j] += g_StudioRenderState.boneAdj[dofInfo->controller];
        }
    }
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

    //

    dstudioevent10_t *events = (dstudioevent10_t *)(header + pDesc->eventindex);

    for (int i = 0; i < pDesc->numevents; i++)
    {
        m_vEvents.push_back(StudioEventV10(&events[i]));
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

int StudioSeqDescV10::SequenceGroup()
{
    return m_iSeqGroup;
}

int StudioSeqDescV10::MotionType()
{
    return m_iMotionType;
}

int StudioSeqDescV10::MotionBone()
{
    return m_iMotionBone;
}

int StudioSeqDescV10::NumFrames()
{
    return m_iNumFrames;
}

float GoldSource::StudioSeqDescV10::FPS()
{
    return m_flFps;
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

StudioBodyPartV10::StudioBodyPartV10(StudioModelV10 *pModel, dstudiobodypart10_t *pBodyPart)
{
    byte *hdr = pModel->Header();

    strlcpy(m_strName, pBodyPart->name, sizeof(m_strName));
    m_iBase = pBodyPart->base;

    dstudiomodel10_t *models = (dstudiomodel10_t *)(hdr + pBodyPart->modelindex);

    m_vModels.reserve(pBodyPart->nummodels);

    for (int i = 0; i < pBodyPart->nummodels; i++)
    {
        m_vModels.push_back(new StudioSubModelV10(pModel, &models[i]));
    }
}

const int StudioBodyPartV10::Base() const
{
    return m_iBase;
}

const int StudioBodyPartV10::NumModels() const
{
    return (int)m_vModels.size();
}

StudioMeshV10::StudioMeshV10(StudioModelV10 *pModel, StudioSubModelV10 *pSubModel, dstudiomesh10_t *pMesh)
{
    numtris   = pMesh->numtris;
    triindex  = pMesh->triindex;
    skinref   = pMesh->skinref;
    numnorms  = pMesh->numnorms;
    normindex = pMesh->normindex;

    m_pModel    = pModel;
    m_pSubmodel = pSubModel;

    assert(_CrtCheckMemory());
    BuildDrawMesh();
    assert(_CrtCheckMemory());
}

StudioMeshV10::~StudioMeshV10()
{
    if (m_pDrawMesh)
        delete m_pDrawMesh;
}

void StudioMeshV10::BuildDrawMesh()
{
    int                i = 0;
    float              s, t;
    dstudiotricmd10_t *drawCmd;

    CheckCorruption();
    m_pDrawMesh = new DrawMesh;
    CheckCorruption();

    m_pDrawMesh->Begin(GL_TRIANGLES);

    short *triCmds = (short *)((byte *)m_pModel->Header() + triindex);

    short             textureIdx = m_pModel->GetSkinRef(skinref);
    StudioTextureV10 *pTexture   = m_pModel->GetTexture(textureIdx);

    s = 1.0 / (float)pTexture->Width();
    t = 1.0 / (float)pTexture->Height();

    auto &verts   = m_pSubmodel->GetVertices();
    auto &normals = m_pSubmodel->GetNormals();

    size_t numVerts = 0;

    while (i = *triCmds++)
    {
        int  vertexState = 0;
        bool strip       = false;

        if (i < 0)
        {
            i = -i;
        }
        else
        {
            strip = true;
        }

        for (; i > 0; i--, triCmds += 4)
        {
            // studioVertexFormat_t *vert = &m_vPreparedVertices[drawCmd->vertexIndex];

            //             vert->uv[0] = drawCmd->s * s;
            //             vert->uv[1] = drawCmd->t * t;
            //
            //             VectorCopy(pStudioNormals[drawCmd->normalIndex], vert->normal);
            //             VectorCopy(pStudioVerts[drawCmd->vertexIndex], vert->position);

            drawCmd = (dstudiotricmd10_t *)triCmds;

            if (vertexState++ < 3)
            {
                m_pDrawMesh->Element1i(numVerts);
            }
            else if (strip)
            {
                uint32_t lastElement         = numVerts - 1;
                uint32_t secondToLastElement = numVerts - 2;

                if (vertexState & 1)
                {
                    m_pDrawMesh->Element1i(secondToLastElement);
                    m_pDrawMesh->Element1i(lastElement);
                    m_pDrawMesh->Element1i(numVerts);
                }
                else
                {
                    m_pDrawMesh->Element1i(lastElement);
                    m_pDrawMesh->Element1i(secondToLastElement);
                    m_pDrawMesh->Element1i(numVerts);
                }
            }
            else
            {
                m_pDrawMesh->Element1i(numVerts - (vertexState - 1));
                m_pDrawMesh->Element1i(numVerts - 1);
                m_pDrawMesh->Element1i(numVerts);
            }

            // m_pDrawMesh->

            auto &vertInfo = verts[drawCmd->vertindex];

            m_pDrawMesh->TexCoord2f(drawCmd->s * s, drawCmd->t * t);
            m_pDrawMesh->Bone1ub(0, vertInfo.bone);
            m_pDrawMesh->Vertex3fv((float *)&vertInfo.origin);
            numVerts++;
        }
    }

    m_pDrawMesh->End();
}

void GoldSource::StudioMeshV10::DrawPoints(StudioEntityState *pState)
{
    auto shader = GLBackend::Instance()->QueryShader("res/glprogs/studio.glsl", {"USING_BONES"});

    short             textureIdx = m_pModel->GetSkinRef(skinref);
    StudioTextureV10 *pTexture   = m_pModel->GetTexture(textureIdx);
    GLBackend::BindTexture(0, pTexture->GetGLTexture());

    shader->Bind();

    for (auto &it : shader->Uniforms())
    {
        switch (it->Kind())
        {
        case UniformKind::BonesTransform:
            // TODO: send actual bones count
            it->SetMat4Array(g_StudioRenderState.boneTransform, 128);
            break;
        case UniformKind::TransformMatrix: {
            glm::mat4 offset = glm::translate(glm::mat4(1.f), pState->origin);
            it->SetMat4(offset);
        }

        break;
        case UniformKind::Diffuse:
            it->SetInt(0);
            break;
        case UniformKind::ObjectSerialNumber:
            it->SetInt(0);
            break;
        default:
            GLBackend::SetUniformValue(it);
            break;
        }
    }

    m_pDrawMesh->BindAndDraw();
}

StudioSubModelV10::StudioSubModelV10(StudioModelV10 *pMainModel, dstudiomodel10_t *pModel)
{
    byte *header = pMainModel->Header();
    strlcpy(m_strName, pModel->name, sizeof(m_strName));

    // Vertices

    glm::vec3 *verts     = (glm::vec3 *)(pMainModel->Header() + pModel->vertindex);
    byte *     vertBones = (pMainModel->Header() + pModel->vertinfoindex);

    m_vVerts.reserve(pModel->numverts);

    for (int i = 0; i < pModel->numverts; i++)
    {
        m_vVerts.push_back(subModelVert(verts[i], vertBones[i]));
    }

    // Normals

    glm::vec3 *normals   = (glm::vec3 *)(pMainModel->Header() + pModel->normindex);
    byte *     normBones = (pMainModel->Header() + pModel->norminfoindex);

    m_vNorms.reserve(pModel->numnorms);

    for (int i = 0; i < pModel->numverts; i++)
    {
        m_vNorms.push_back(subModelNorm(verts[i], vertBones[i]));
    }

    dstudiomesh10_s *meshes = (dstudiomesh10_t *)(header + pModel->meshindex);

    for (int i = 0; i < pModel->nummesh; i++)
    {
        assert(_CrtCheckMemory());
        m_vMeshes.push_back(new StudioMeshV10(pMainModel, this, &meshes[i]));
    }
}

void StudioSubModelV10::DrawPoints(StudioEntityState *pState) const
{
    for (auto &it : m_vMeshes)
    {
        it->DrawPoints(pState);
    }
}

StudioSubModelV10::~StudioSubModelV10()
{
    ClearPointersVector(m_vMeshes);
}

StudioTextureV10::StudioTextureV10(byte *header, dstudiotexture10_t *pTexture)
{
    strlcpy(m_strName, pTexture->name, sizeof(m_strName));
    m_iFlags  = pTexture->flags;
    m_iWidth  = pTexture->width;
    m_iHeight = pTexture->height;

    byte *textureData = header + pTexture->index;
    byte *pallete     = header + pTexture->index + (pTexture->width * pTexture->height);

    m_pPixels  = new IndexedFromMemoryImage(m_iWidth, m_iHeight, textureData, pallete);
    m_pTexture = TextureManager::LoadTextureAsynch(m_pPixels, 0, m_strName, TextureSource::IndexedFrommemory);
}

int StudioTextureV10::Width()
{
    return m_iWidth;
}

int StudioTextureV10::Height()
{
    return m_iHeight;
}

GLTexture *StudioTextureV10::GetGLTexture()
{
    return m_pTexture;
}

StudioTextureV10::~StudioTextureV10()
{
    if (m_pTexture)
        TextureManager::Instance()->DestroyTexture(m_pTexture);

    if (m_pPixels)
        delete m_pPixels;
}

GoldSource::StudioSeqGroupV10::StudioSeqGroupV10(byte *hdr, dstudioseqgroup10_t *pGroup)
{
    strlcpy(m_strLabel, pGroup->label, sizeof(m_strLabel));
    strlcpy(m_strName, pGroup->name, sizeof(m_strName));

    // TODO: figure out how to copy only required data
    m_pData = hdr + pGroup->data;
}

dstudioanim10_t *StudioSeqGroupV10::GetAnimation(StudioSeqDescV10 *pSeqDesc)
{
    if (pSeqDesc->SequenceGroup() == 0)
    {
        return (dstudioanim10_t *)((byte *)m_pData + pSeqDesc->AnimsIndex());
    }

    return nullptr;

    // return (dstudioanim10_t *)((byte *)m_panimhdr[pseqdesc->seqgroup] + pseqdesc->animindex);
}

StudioAttachmentV10::StudioAttachmentV10(dstudioattachment10_t *pAttachment)
{
    strlcpy(m_strName, pAttachment->name, sizeof(m_strName));
    m_iBone = pAttachment->bone;

    m_iType = pAttachment->type;

    m_Origin = pAttachment->org;
    for (int i = 0; i < 3; i++)
        m_Vectors[i] = pAttachment->vectors[i];
}

int StudioBoneControllerV10::Index()
{
    return index;
}

int StudioBoneControllerV10::Type()
{
    return type;
}

float StudioBoneControllerV10::Start()
{
    return start;
}

float StudioBoneControllerV10::End()
{
    return end;
}
