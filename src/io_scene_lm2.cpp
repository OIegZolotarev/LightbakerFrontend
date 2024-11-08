/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#define _CRT_SECURE_NO_WARNINGS

#include "application.h"
#include "common.h"

#include "io_scene_lm2.h"



int SceneIOLm2::Caps()
{
    return SCENE_IO_CAPS_CAMERAS;
}

const char *SceneIOLm2::Description() const
{
    return "Lb3k frontend scene (*.lm2)";
}

void SceneIOLm2::Deserialize(Scene *pNewScene)
{
    
}

void SceneIOLm2::Serialize(const char* fileName, Scene *pScene)
{
    m_keyValues.clear();
    InitStringPool();

    m_pCurrentFile = fopen(fileName, "wb");

    if (!m_pCurrentFile)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error while saving", "Can't open file for writing", 0);
        return;
    }

    dlm2_header_t hdr = {0};

    memcpy(hdr.magic, "LMAO", 4);
    hdr.num_lumps = LM2_NUM_LUMPS;
    
    lm2_lump_t lumps[LM2_NUM_LUMPS] = {0};
    
    fwrite(&hdr, sizeof(hdr), 1, m_pCurrentFile);

    size_t lumpsOffset = ftell(m_pCurrentFile);

    fwrite(&lumps, sizeof(lm2_lump_t), LM2_NUM_LUMPS, m_pCurrentFile);

    WriteSceneCameras(&lumps[LM2_LUMP_SCENE_CAMERAS], pScene);    
    WriteEntities(&lumps[LM2_LUMP_ENTITIES], pScene);

    FinishExport(lumps, lumpsOffset);
}

void SceneIOLm2::FinishExport(lm2_lump_t *lumps, size_t lumpsOffset)
{
    // Should be called last!
    WriteStringsPool(&lumps[LM2_LUMP_STRINGS]);

    fseek(m_pCurrentFile, lumpsOffset, SEEK_SET);
    fwrite(&lumps, sizeof(lm2_lump_t), LM2_NUM_LUMPS, m_pCurrentFile);

    fclose(m_pCurrentFile);
}

void SceneIOLm2::InitStringPool()
{
    m_StringsPool.Reset();
}

bool SceneIOLm2::IsFileFormatSupported(const char *fileName)
{
    FileSystem::ExtractFileExtension(fileName);
    return true;
}

SceneIOClasses SceneIOLm2::FormatID()
{
    return SceneIOClasses::LM2;
}

void SceneIOLm2::WriteSceneCameras(lm2_lump_t * lump, Scene *pScene)
{
    lump->offset = ftell(m_pCurrentFile);

    auto & allCameras = pScene->GetSceneCamerasDescriptors();

    for (auto & camDescriptor: allCameras)
    {
        dlm2_scene_camera_t descr = {};
        
        descr.position = camDescriptor.position;
        descr.angles = camDescriptor.angles;
        descr.name = m_StringsPool.AllocString(camDescriptor.description.c_str());

        fwrite(&descr, sizeof(descr), 1, m_pCurrentFile);
    }

    lump->length = ftell(m_pCurrentFile) - lump->offset;
}

void SceneIOLm2::WriteStringsPool(lm2_lump_t * lump)
{
    lump->offset = ftell(m_pCurrentFile);
    fwrite(m_StringsPool.Data(), m_StringsPool.Length(), 1, m_pCurrentFile);
    lump->length = ftell(m_pCurrentFile) - lump->offset;
}

void SceneIOLm2::WriteEntities(lm2_lump_t *lump, Scene * pScene)
{
    lump->offset = ftell(m_pCurrentFile);

    auto & entities = pScene->GetSceneObjects();
    
    for (auto &it : entities)
    {
        dlm2_entity_t entity;

        entity.firstKey = m_keyValues.size();
        entity.numKeys  = 0;

        for (auto & prop : it->GetProperties())
        {
            auto propValue = prop->SerializeValue();
            auto & propName  = prop->Name();

            AllocKeyValue(propName.c_str(), propValue.c_str());
            entity.numKeys++;
        }



    }

    fwrite(m_keyValues.data(), sizeof(dlm2_entity_t), entities.size(), m_pCurrentFile);

    lump->length = ftell(m_pCurrentFile) - lump->offset;
}

size_t SceneIOLm2::AllocKeyValue(const char *key, const char *value)
{
    lm2string_t keyIdx = m_StringsPool.AllocString(key);
    lm2string_t valueIdx = m_StringsPool.AllocString(value);

    dlm2_keyvalue_t r;
    r.key = keyIdx;
    r.value = valueIdx;

    m_keyValues.push_back(r);

    return m_keyValues.size() - 1;
}
