/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once

#include "common.h"

class StringPool
{
    std::vector<char> m_vecData;
public:
    StringPool(size_t initialSize = 16384);

    void Reset();

    const size_t AllocString(const char *str);
    const char * GetString(const size_t index) const;

    const char *Data() const;
    const size_t Length();
};
