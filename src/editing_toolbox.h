/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/
#pragma once

#include "common.h"
#include "editing_tool.h"
#include "event_handler.h"
#include "helpers.h"

class EditingToolbox : public Singleton<EditingToolbox>, public IEventHandler
{
    std::list<IEditingTool *> m_lstToolbs;

    IEditingTool *m_pCurrentTool = nullptr;

    IEditingTool *FindTool(EditingToolId id);

public:
    ~EditingToolbox();
    void Initialize();
    void SelectTool(EditingToolId toolId);

    void RenderToolViewportUI(Viewport *pViewport);
    void RenderToolViewport2DGraphics(float flFrameDelta);

    void RenderTool(float flFrameDelta);
    void RenderToolUI();

    int HandleEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta) override;

    void InitializeToolContext();

    const std::list<IEditingTool *> &GetAllTools() const;
    const EditingToolId              SelectedToolId() const;

    int Priority() override;
};
