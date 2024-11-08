/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once
#include "io_scene.h"
#include "string_pool.h"

#define LM2_HEADER      "LMAO"
#define LM2_CURRENT_VER 1
#define LM2_NUM_LUMPS   4

#define LM2_LUMP_STRINGS       0
#define LM2_LUMP_SCENE_CAMERAS 1
#define LM2_LUMP_ENTITIES      2
#define LM2_LUMP_KEY_VALUES    3

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

typedef struct dlm2_scene_camera_s
{
    glm::vec3 position;
    glm::vec3 angles;

    lm2string_t name;
} dlm2_scene_camera_t;

typedef struct dlm2_keyvalue_s
{
    lm2string_t key;
    lm2string_t value;
} dlm2_keyvalue_t;

typedef struct dlm2_entity_s
{
    uint32_t firstKey;
    uint32_t numKeys;
} dlm2_entity_t;

class SceneIOLm2 : public SceneIO
{
    StringPool m_StringsPool;

    std::vector<dlm2_keyvalue_t> m_keyValues;

    FILE *m_pCurrentFile;

    void WriteSceneCameras(lm2_lump_t *lump, Scene *pScene);
    void WriteStringsPool(lm2_lump_t *lump);
    void WriteEntities(lm2_lump_t *lump, Scene *pScene);

    size_t AllocKeyValue(const char *key, const char *value);

public:
    int         Caps() override;
    const char *Description() const override;

    void Deserialize(Scene *pNewScene) override;
    void Serialize(const char *fileName, Scene *pScene) override;

    void FinishExport(lm2_lump_t *lumps, size_t lumpsOffset);

    void InitStringPool();

protected:
    SceneIOClasses FormatID() override;
    bool           IsFileFormatSupported(const char *fileName) override;
};
