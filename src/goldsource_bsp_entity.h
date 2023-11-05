/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include <utility>
#include <unordered_map>

namespace GoldSource
{

class BSPEntity
{
    typedef std::pair<std::string, std::string> kvData;

    std::unordered_map<std::string, std::string> m_vProperties;

public:
    BSPEntity();
    ~BSPEntity();
    void SetKeyValue(std::string& key, std::string& value);
};


}

