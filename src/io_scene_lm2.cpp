/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#define _CRT_SECURE_NO_WARNINGS

#include "application.h"
#include "common.h"

#include "io_scene_lm2.h"

size_t SceneIOLm2::AllocStringInPool(const char *str)
{
    size_t length = strlen(str);

    m_stringsPool.push_back(std::string(str));
    m_stringPoolOffset += (length + 1);
}

int SceneIOLm2::Caps()
{
    return SCENE_IO_CAPS_CAMERAS;
}

const char *SceneIOLm2::Description()
{
    return "Lb3k frontend scene (*.lm2)";
}

void SceneIOLm2::Deserialize(Scene *pNewScene)
{
    
}

void SceneIOLm2::Serialize(const char* fileName, Scene *pScene)
{
    InitStringPool();

    FILE *fp = fopen(fileName, "wb");

    if (!fp)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error while saving", "Can't open file for writing", 0);
        return;
    }

    dlm2_header_t hdr = {0};

    memcpy(hdr.magic, "LMAO", 4);
    hdr.num_lumps = LM2_NUM_LUMPS;

    lm2_lump_t lumps[LM2_NUM_LUMPS] = {0};

    fwrite(&hdr, sizeof(hdr), 1, fp);

    size_t lumpsOffset = ftell(fp);

    fwrite(&lumps, sizeof(lm2_lump_t), LM2_NUM_LUMPS, fp);

    WriteSceneCameras(fp, &lumps[LM2_LUMP_SCENE_CAMERAS], pScene);
    WriteStringsPool(fp, &lumps[LM2_LUMP_STRINGS]);


    fseek(fp, lumpsOffset, SEEK_SET);
    fwrite(&lumps, sizeof(lm2_lump_t), LM2_NUM_LUMPS, fp);

    fclose(fp);

}

void SceneIOLm2::InitStringPool()
{
    m_stringPoolOffset = 0;
    m_stringsPool.clear();
}

void SceneIOLm2::WriteSceneCameras(FILE *fp, lm2_lump_t * lump, Scene *pScene)
{
    lump->offset = ftell(fp);

    auto & allCameras = pScene->GetSceneCamerasDescriptors();

    for (auto & camDescriptor: allCameras)
    {
        dlm2_scene_camera_t descr = {};
        
        descr.position = camDescriptor.position;
        descr.angles = camDescriptor.angles;
        descr.name = AllocStringInPool(camDescriptor.description.c_str());

        fwrite(&descr, sizeof(descr), 1, fp);
    }

    lump->length = ftell(fp) - lump->offset;
}

void SceneIOLm2::WriteStringsPool(FILE *fp, lm2_lump_t * lump)
{
    lump->offset = ftell(fp);

    byte null = 0;

    for (auto & it: m_stringsPool)
    {
        fwrite(it.c_str(), 1, it.length(), fp);
        fwrite(&null, 1, 1, fp);
    }

    lump->length = ftell(fp) - lump->offset;
}
