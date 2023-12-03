/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "application.h"
#include "debug_panel.h"

DebugPanel::DebugPanel() : IGUIPanel(PanelsId::Debug, (char*)"Debug")
{

}

DockPanels DebugPanel::GetDockSide()
{
	return DockPanels::BottomLeft;
}

void DebugPanel::Render()
{
    Viewport *pViewport1 = Application::GetMainWindow()->GetViewport(0);
    Viewport *pViewport2 = Application::GetMainWindow()->GetViewport(1);

	int fps = Application::GetMainWindow()->GetFPS();

	ImGui::Begin("Debug");

	//ImGui::LoadIniSettingsFromDisk

	ImGui::Text("FPS: %d\n", fps);

	auto *stats = GLBackend::Instance()->RenderStats();

	ImGui::Text("DrawCalls: %d\n", stats->nDrawCalls);
	ImGui::Text("Triangles: %d\n", stats->nTriangles);
	
	pViewport1->OutputDebug();
    pViewport2->OutputDebug();

	ImGui::End();
}
