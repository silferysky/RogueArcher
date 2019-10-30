#include "EditorViewport.h"
#include "Main.h"

namespace Rogue
{
	ImGuiEditorViewport::ImGuiEditorViewport()
	{
	}
	ImGuiEditorViewport::~ImGuiEditorViewport()
	{
	}
	void ImGuiEditorViewport::Init()
	{

	}
	void ImGuiEditorViewport::Update()
	{
		ImGui::Begin("Viewport");
		if (ImGui::Button("Play"))
		{
			//If game is paused, save level and load it again, and unpause it
			if (g_engine.m_coordinator.GetPauseState())
			{
				g_engine.m_coordinator.GetSceneManager().SaveAndLoadLevel();
				g_engine.m_coordinator.SetPauseState(false);
			}
			else //If game is running, just stop
			{
				//Loads last iteration and pauses game
				g_engine.m_coordinator.GetSceneManager().ClearAllEntities();
				g_engine.m_coordinator.GetSceneManager().LoadLevel(g_engine.m_coordinator.GetSceneManager().getCurrentFileName().c_str());
				g_engine.m_coordinator.SetPauseState(true);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			//Pause/Unpause
			g_engine.m_coordinator.TogglePauseState();
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			if (!g_engine.m_coordinator.GetPauseState())
			{
				//Loads last iteration and pauses game
				g_engine.m_coordinator.GetSceneManager().ClearAllEntities();
				g_engine.m_coordinator.GetSceneManager().LoadLevel(g_engine.m_coordinator.GetSceneManager().getCurrentFileName().c_str());
				g_engine.m_coordinator.SetPauseState(true);
			}
		}

		ImVec2 imageSize{ ImGui::GetContentRegionAvail() };
		ImGui::Image((void*)(intptr_t)(g_engine.m_coordinator.GetSystem<GraphicsSystem>()->getFBO()), ImVec2(imageSize.x,imageSize.y ), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}
	void ImGuiEditorViewport::Shutdown()
	{

	}
}