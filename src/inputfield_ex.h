/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/


#pragma once

#include "common.h"

enum class HeaderPosition
{
    Left = 0,
    Right,
    Top,
    None
};

#define FL_CHOOSE_FILE_BUTTON (1<<0)
#define FL_CLEAR_BUTTON (1<<1)

#define FL_FILE_SELECTOR (FL_CHOOSE_FILE_BUTTON | FL_CLEAR_BUTTON)

class InputFieldEx
{    
    std::string m_Header;
    std::string m_ImGUIIdHeader;

    std::string m_FileDialogTitle;
    std::string m_FileDialogFilters;

    std::string & m_Container;

    HeaderPosition m_HeaderPos;

    enum class Buttons
    {
        Clear,
        ChooseFile
    };

 public:
    InputFieldEx(const char* title, std::string & container, unsigned int flags  = 0);
    ~InputFieldEx();

    void SetHeaderPosition(HeaderPosition pos);
    void RenderGui();

    void SetFileDialogTitle(std::string title);

    void SetFileDialogFilter(std::string filter);

  private:
    unsigned int m_iFlags;
    void OnButtonPressed(Buttons id);
};
