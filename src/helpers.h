/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include <memory>

typedef unsigned char byte;

#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))

#ifdef _MSC_VER
// not #if defined(_WIN32) || defined(_WIN64) because we have strncasecmp in mingw
#define strncasecmp _strnicmp
#define strcasecmp  _stricmp
#endif

template <class T> inline void ClearPointersVector(T &vec)
{
    for (auto &it : vec)
        delete it;

    vec.clear();
    vec.shrink_to_fit();
}

template <class T> inline void FreeVector(T &vec)
{
    vec.clear();
    vec.shrink_to_fit();
}

// Weak ptr comparsion
template <typename T, typename U> inline bool equals(const std::weak_ptr<T> &t, const std::shared_ptr<U> &u)
{
    return !t.owner_before(u) && !u.owner_before(t);
}

template <typename T, typename D = T> class Singleton
{
    friend D;
    static_assert(std::is_base_of_v<T, D>, "T should be a base type for D");

  public:
    static T * Instance()
    {
        static D * inst = new D;
        return inst;
    }

  private:
    Singleton()                  = default;
    ~Singleton()                 = default;
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;
};