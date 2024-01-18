/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once

#include "common.h"


// Vector is 2 times faster in debug and about 4 times faster in release
template<class T>
using  map220ContainerType = std::vector<T>;

typedef struct map220face_s
{
    glm::vec3   points[3];
    std::string textureName;

    glm::vec4 uAxis;
    glm::vec4 vAxis;

    float     rotation;
    glm::vec2 scale;

    map220face_s()
    {
        points[0] = glm::vec3(0.f, 0.f, 0.f);
        points[1] = glm::vec3(0.f, 0.f, 0.f);
        points[2] = glm::vec3(0.f, 0.f, 0.f);

        textureName = "NULL";

        uAxis    = glm::vec4(0, 0, 0, 0);
        vAxis    = glm::vec4(0, 0, 0, 0);
        rotation = 0;
        scale    = glm::vec2(1, 1);
    }

    map220face_s(const glm::vec3 pt1, const glm::vec3 pt2, const glm::vec3 pt3, const std::string &_textureName,
                 const glm::vec4 _uAxis, const glm::vec4 _vAxis, float _rotation, glm::vec2 _scale)

    {
        points[0] = pt1;
        points[1] = pt2;
        points[2] = pt3;

        textureName = _textureName;
        uAxis       = _uAxis;
        vAxis       = _vAxis;

        rotation = _rotation;
        scale    = _scale;
    }
} map220face_t;

typedef struct map220keyvalue_s
{
    // Keep same fields order!!
    std::string key;
    std::string value;
} map220keyvalue_t;

typedef struct map220brush_s
{
    map220ContainerType<map220face_t *> faces;
} map220brush_t;

typedef struct map220_entity_s
{
    // Keep same fields order!!
    map220ContainerType<map220keyvalue_t *> properties;
    map220ContainerType<map220brush_t *>    brushes;

    map220_entity_s(map220ContainerType<map220keyvalue_t *> &_properties,
                    map220ContainerType<map220brush_t *> &   _brushes)
    {
        properties = std::move(_properties);
        brushes    = std::move(_brushes);
    }

} map220_entity_t;

class HammerMap
{
    FileData *                 m_pFileData;
    map220ContainerType<map220_entity_t *> m_lstEntities;

public:
    HammerMap(FileData *pFileData);
    ~HammerMap();

    const std::string &FileName() const;
    const char *       Data() const;
    const size_t       Length() const;

    void AddEntity(map220_entity_t *entity);
};
