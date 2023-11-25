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

class LoadFileDialog : public IImGUIPopup
{
    std::string m_Title;
    std::string m_Filters;

    pfnOnSelectCallback m_LoadCallback;

  public:
    LoadFileDialog();
    ~LoadFileDialog();

    void Render() override;

    bool BeginRendering() override;
    void EndRendering() override;


    void SetTitle(const char *title);
    void SetFilters(const char *filters);
    void SetOnSelectCallback(pfnOnSelectCallback callback);

    static LoadFileDialog *Instance();


  protected:
    void OpenPopup() override;
};
