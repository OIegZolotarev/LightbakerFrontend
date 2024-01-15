/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once

#include <glad/glad.h>

#define IMGUI_DEFINE_MATH_OPERATORS

#include <SDL2/SDL.h>

#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include <ImGuiFileDialog/ImGuiFileDialog.h>
#include <ImGuizmo/ImGuizmo.h>

#include <glm/fwd.hpp>

glm::vec2 ToGLMVec2(ImVec2 pt);

glm::vec4 ToGLMVec4(ImVec4 pt);

ImVec2 FromGLMVec2(glm::vec2 pt);

ImVec4 FromGLMVec4(glm::vec4 pt);