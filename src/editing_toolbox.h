/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/
#pragma once

#include "common.h"
#include "helpers.h"
#include "editing_tool.h"
#include "event_handler.h"


class EditingToolbox: public Singleton<EditingToolbox>, 
    IEventHandler
{
    std::list<IEditingTool *> m_lstToolbs;

    IEditingTool *m_pCurrentTool = nullptr;

    IEditingTool *FindTool(EditingToolId id);

public:
    void Initialize();
    void SelectTool(EditingToolId toolId);
       

    void RenderTool(float flFrameDelta);
    void RenderToolUI();

    int HandleEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta) override;

    const std::list<IEditingTool *> & GetAllTools() const;
    const EditingToolId                    SelectedToolId() const;
};
