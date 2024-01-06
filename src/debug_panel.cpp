/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#include "debug_panel.h"
#include "application.h"
#include "btQuickProf.h"
#include "viewports_orchestrator.h"

extern bool g_useBVH;

DebugPanel::DebugPanel() : ToolUIPanel(ToolUIPanelID::Debug, (char *)"Debug")
{
}

DockPanels DebugPanel::GetDockSide()
{
    return DockPanels::BottomLeft;
}

void iterateRecursive(CProfileIterator *profileIterator, int level = 0)
{
    profileIterator->First();
    if (profileIterator->Is_Done())
        return;

    float accumulated_time = 0, parent_time = profileIterator->Is_Root()
                                                  ? CProfileManager::Get_Time_Since_Reset()
                                                  : profileIterator->Get_Current_Parent_Total_Time();
    int   i;
    int   frames_since_reset = CProfileManager::Get_Frame_Count_Since_Reset();

    //     if (ImGui::TreeNode(profileIterator->Get_Current_Parent_Name(), "Profiling: %s (total running time: %.3f ms)
    //     ---",
    //                         profileIterator->Get_Current_Parent_Name(), parent_time))
    //{
    float totalTime = 0.f;

    int numChildren = 0;

    for (i = 0; !profileIterator->Is_Done(); i++, profileIterator->Next())
    {
        numChildren++;
        float current_total_time = profileIterator->Get_Current_Total_Time();
        accumulated_time += current_total_time;
        float fraction = parent_time > FLT_EPSILON ? (current_total_time / parent_time) * 100 : 0.f;

        totalTime += current_total_time;
        // recurse into children

        if (ImGui::TreeNode(
                profileIterator->Get_Current_Name(), "%d -- [%d] %s (%.2f %%) :: %.3f ms / frame (%d calls)", i, level,
                profileIterator->Get_Current_Name(), fraction, (current_total_time / (double)frames_since_reset),
                profileIterator->Get_Current_Total_Calls()))
        {
            CProfileIterator iter2 = CProfileIterator(*profileIterator);

            iter2.Enter_Child(i);
            iterateRecursive(&iter2, (level + 1));

            ImGui::TreePop();
        }
    }

    // ImGui::TreePop();
    //}

    ImGui::Text("%s (%.3f %%) \n", "Unaccounted:",
                parent_time > FLT_EPSILON ? ((parent_time - accumulated_time) / parent_time) * 100 : 0.f);
}

void DebugPanel::Render()
{
    Viewport *pViewport1 = Application::GetMainWindow()->GetViewport(0);
    Viewport *pViewport2 = Application::GetMainWindow()->GetViewport(1);

    int fps = Application::GetMainWindow()->GetFPS();

    if (ImGui::Begin("Debug"))
    {
        ImGui::Text("FPS: %d\n", fps);

        auto *stats = GLBackend::Instance()->RenderStats();

        ImGui::Text("DrawCalls: %d\n", stats->nDrawCalls);
        ImGui::Text("Triangles: %d\n", stats->nTriangles);

        ImGui::Text("Entities rendered/total: %d/%d\n", stats->nEntitiesRendered, stats->nEntitiesTotal);

        ImGui::Text("Shaders switches (total/unnecessary): %d/%d\n", stats->nShaderBinds,
                    stats->nUnnecessaryShaderBinds);

        auto iterator = CProfileManager::Get_Iterator();

        iterator->First();
        if (iterator->Is_Done())
            return;

        float accumulated_time = 0, parent_time = iterator->Is_Root() ? CProfileManager::Get_Time_Since_Reset()
                                                                      : iterator->Get_Current_Parent_Total_Time();
      
        int   frames_since_reset = CProfileManager::Get_Frame_Count_Since_Reset();

        if (ImGui::TreeNode(iterator->Get_Current_Parent_Name(), "Profiling: %s (total running time: %.3f ms) ---",
                            iterator->Get_Current_Parent_Name(), parent_time))
        {
            iterateRecursive(iterator);
            ImGui::TreePop();
        }

        CProfileManager::Release_Iterator(iterator);

        static bool forceUpdates = false;

        ImGui::Checkbox("Force viewport updates", &forceUpdates);

        if (forceUpdates)
            ViewportsOrchestrator::Instance()->FlagRepaintAll();

        ImGui::Checkbox("Use BVH", &g_useBVH);

        ImGui::SameLine();

        if (ImGui::Button("Reset samples"))
        {
            CProfileManager::Reset();
        }
    }
    ImGui::End();
}


