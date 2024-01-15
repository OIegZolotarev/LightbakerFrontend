/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once
#include "io_scene.h"

#define LM2_HEADER      "LMAO"
#define LM2_CURRENT_VER 1
#define LM2_NUM_LUMPS   2

typedef struct lm2_lump_s
{
    uint32_t offset;
    uint32_t length;
} lm2_lump_t;

// Index into strings pool
typedef uint32_t lm2string_t;

typedef struct dlm2_header_s
{
    char magic[4];

    uint32_t version;
    uint32_t num_lumps;

} dlm2_header_t;

#define LM2_LUMP_STRINGS       0
#define LM2_LUMP_SCENE_CAMERAS 1
#define LM2_LUMP_ENTITIES      2

typedef struct dlm2_scene_camera_s
{
    glm::vec3 position;
    glm::vec3 angles;

    lm2string_t name;

} dlm2_scene_camera_t;

class SceneIOLm2 : public SceneIO
{
    std::vector<std::string> m_stringsPool;

    size_t m_stringPoolOffset = 0;
    size_t AllocStringInPool(const char *str);

    void WriteSceneCameras(FILE *fp, lm2_lump_t *lump, Scene *pScene);
    void WriteStringsPool(FILE *fp, lm2_lump_t *lump);

public:
    int         Caps() override;
    const char *Description() override;

    void Deserialize(Scene *pNewScene) override;
    void Serialize(const char *fileName, Scene *pScene) override;

    void InitStringPool();
};
