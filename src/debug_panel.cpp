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
	ImGui::End();
}
