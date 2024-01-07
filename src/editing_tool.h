/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once

#include "viewport.h"
#include "commands_registry.h"

enum class MouseButtons
{
    Left,
    Right,
    Middle,
    Aux1,
    Aux2
};

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
    Viewport *m_pActiveViewport;
    Scene *   m_pActiveDocument;


    EditingToolId m_id = EditingToolId::None;
    GlobalCommands m_BoundCommand = GlobalCommands::None;
    CommonIcons    m_ToolIcon;
    char           m_strDescription[32];

    friend class EditingToolbox;
    
        
    void RegisterBoundCommand();

protected:
    void SetBoundCommand(GlobalCommands cmd);
    void SetToolIcon(CommonIcons icon);
    void SetDescritpion(const char *strDescription);
 public:
    IEditingTool(EditingToolId id);
    ~IEditingTool();

    virtual void OnMouseClicked(Viewport * pActiveViewport, Scene* pActiveDocument);
    virtual void OnMouseReleased();
    
    virtual void Render(float flFrameDelta);
    virtual void RenderUI();
    
    const EditingToolId  GetToolId() const;
    const GlobalCommands GetBoundCommand() const;
    
};
