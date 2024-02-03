/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#pragma once
#include "imgui_popups.h"
#include "lb3k_wrapper.h"
#include <functional>
#include <string>

typedef std::function<void(std::string &fileName)> pfnOnSelectCallback;

class FileDialog : public IImGUIPopup
{
    std::string m_Title;
    std::string m_Filters;

    pfnOnSelectCallback m_LoadCallback;

public:
    FileDialog();
    ~FileDialog();

    void Render() override;

    void SetTitle(const char *title);
    void SetFilters(const char *filters);
    void SetOnSelectCallback(pfnOnSelectCallback callback);

    static FileDialog *Instance();

    void Show();

protected:
    void OpenPopup() override;
};
