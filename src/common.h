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

#include <functional>
#include <list>
#include <stdlib.h>
#include <string>
#include <vector>

#define GLM_FORCE_SWIZZLE
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>

#include <glad/glad.h>

#pragma warning(disable : 4244)

#include "event_handler.h"

//#define NICE_LINES

#include "helpers.h"
