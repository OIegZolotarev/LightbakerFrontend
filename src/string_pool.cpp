/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "string_pool.h"

StringPool::StringPool(size_t initialSize /*= 16384*/)
{
    m_vecData.reserve(initialSize);
}

void StringPool::Reset()
{
    m_vecData.clear();
}

const size_t StringPool::AllocString(const char *str)
{
    size_t offset = m_vecData.size();
    size_t resultOffset = offset;


    char *p = (char *)str;

    while (*p)
    {
        m_vecData[offset++] = *p++;        
    }

    m_vecData[offset] = 0;

    return resultOffset;
}

const char *StringPool::GetString(const size_t index) const
{
    return &m_vecData[index];
}

const char *StringPool::Data() const
{
    return m_vecData.data();
}

const size_t StringPool::Length()
{
    return m_vecData.size();
}
