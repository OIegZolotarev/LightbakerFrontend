/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once

#include "commands_registry.h"
#include "viewport.h"

enum class EditingToolId
{
    Selection,
    Camera,
    Brushwork,
    Entities,
    None
};

class IEditingTool
{
    EditingToolId  m_id           = EditingToolId::None;
    GlobalCommands m_BoundCommand = GlobalCommands::None;
    CommonIcons    m_ToolIcon;

    char m_strDescription[32];
    friend class EditingToolbox;

    void RegisterBoundCommand();

protected:

    Viewport *m_pActiveViewport;
    Scene *   m_pActiveDocument;

    void        SetBoundCommand(GlobalCommands cmd);
    void        SetToolIcon(CommonIcons icon);
    void        SetDescritpion(const char *strDescription);
    const char *GetDescrition() const;

public:
    IEditingTool(EditingToolId id);
    ~IEditingTool();

    virtual int HandleMouseEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta);
    virtual int HandleKeyboardEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta);

    virtual void Render(float flFrameDelta);
    virtual void RenderUI();

    const EditingToolId  GetToolId() const;
    const GlobalCommands GetBoundCommand() const;

    void SetActiveViewport(Viewport *pViewport);
    void SetActiveDocument(Scene *pScene);
};
