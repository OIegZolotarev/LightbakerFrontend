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
    auto it = m_ExistingValues.find(std::string(str));

    if (it != m_ExistingValues.end())
    {
        return (*it).second;
    }

    size_t offset = m_vecData.size();
    size_t resultOffset = offset;


    char *p = (char *)str;

    while (*p)
    {
        m_vecData.push_back(*p++);     
        offset++;
    }

    m_vecData.push_back(0);

    
    m_ExistingValues.insert(std::pair<std::string,size_t>(std::string(str), resultOffset));
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
