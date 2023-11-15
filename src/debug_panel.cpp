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

	int fps = Application::GetMainWindow()->GetFPS();

	ImGui::Begin("Debug");

	//ImGui::LoadIniSettingsFromDisk

	ImGui::Text("FPS: %d\n", fps);

	auto *stats = GLBackend::Instance()->RenderStats();

	ImGui::Text("DrawCalls: %d\n", stats->nDrawCalls);
	ImGui::Text("Triangles: %d\n", stats->nTriangles);

	ImGui::End();
}
