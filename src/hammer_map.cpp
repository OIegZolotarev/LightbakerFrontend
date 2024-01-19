/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "hammer_map.h"

extern void ParseMap(HammerMap *file);

HammerMap::HammerMap(FileData *pFileData)
{
    m_pFileData = pFileData;
    m_pFileData->Ref();

    ParseMap(this);
}

HammerMap::~HammerMap()
{
    m_pFileData->UnRef();

    ClearPointersVector(m_lstEntities);
    ClearPointersVector(m_lstBrushFaceList);
    ClearPointersVector(m_lstBrushListOpt);
    ClearPointersVector(m_lstEntityPropertiesOpt);
}

const std::string &HammerMap::FileName() const
{
    if (!m_pFileData)
    {
        static std::string sEmpty = "";
        return sEmpty;
    }

    return m_pFileData->Name();
}

const char *HammerMap::Data() const
{
    if (!m_pFileData)
        return nullptr;

    return (const char *)m_pFileData->Data();
}

const size_t HammerMap::Length() const
{
    if (!m_pFileData)
        return 0;

    return m_pFileData->Length();
}

void HammerMap::AddNewEntity()
{
    size_t firstBrush = m_CurrentEntityFirstBrush;
    size_t numBrushes = m_lstBrushListOpt.size() - m_CurrentEntityFirstBrush;

    size_t firstProperty = m_CurrentEntityFirstProperty;
    size_t numProperties   = m_lstEntityPropertiesOpt.size() - m_CurrentEntityFirstProperty;

    map220_entity_t *pNewEntity = new map220_entity_t(firstProperty, numProperties, firstBrush, numBrushes);

    m_CurrentEntityFirstProperty = m_lstEntityPropertiesOpt.size();
    m_CurrentEntityFirstBrush    = m_lstBrushListOpt.size();

    m_lstEntities.push_back(pNewEntity);
}

map220keyvalue_t *HammerMap::MakeNewKeyValue(const std::string &key, const std::string &value)
{
    map220keyvalue_t *result = new map220keyvalue_t(key, value);
    m_lstEntityPropertiesOpt.push_back(result);
    return result;
}

map220brush_t *HammerMap::MakeNewBrush()
{
    size_t firstFace = m_CurrentBrushFirstFace;
    size_t numFaces  = m_lstBrushFaceList.size() - m_CurrentBrushFirstFace;

    if (numFaces == 0)
    {
        // assert(false);
        return nullptr;
    }

    m_CurrentBrushFirstFace = m_lstBrushFaceList.size();
    map220brush_t *result   = new map220brush_t(firstFace, numFaces);

    m_lstBrushListOpt.push_back(result);

    return result;
}

map220face_t *HammerMap::MakeNewFace(const glm::vec3 pt1, const glm::vec3 pt2, const glm::vec3 pt3,
                                     const std::string &_textureName, const glm::vec4 _uAxis, const glm::vec4 _vAxis,
                                     float _rotation, glm::vec2 _scale)
{
    auto result = new map220face_t(pt1, pt2, pt3, _textureName, _uAxis, _vAxis, _rotation, _scale);
    m_lstBrushFaceList.push_back(result);
    return result;
}

map220keyvalue_s::map220keyvalue_s(const std::string &_key, const std::string &_value)
{
    key   = _key;
    value = _value;
}
