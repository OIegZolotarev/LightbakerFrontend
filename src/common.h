/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

// ����� �����

#define NOMINMAX



#ifdef WIN32
#include <windows.h>
#endif


#include <stdlib.h>
#include <vector>
#include <list>
#include <string>
#include <functional>

#define GLM_FORCE_SWIZZLE 
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include <glad/glad.h>

#pragma warning(disable:4244)

typedef unsigned char byte;

template<class T>
inline void ClearPointersVector(T & vec)
{
	for (auto& it : vec)
		delete it;

	vec.clear();
	vec.shrink_to_fit();
}

template<class T>
inline void FreeVector(T& vec)
{
	vec.clear();
	vec.shrink_to_fit();
}

#include "event_handler.h"

#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))

#define NICE_LINES