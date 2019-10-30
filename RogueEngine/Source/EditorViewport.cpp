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
			//If game is not running, save level, and set it to running
			if (!g_engine.m_coordinator.GetGameState())
			{
				g_engine.m_coordinator.GetSceneManager().SaveLevel(g_engine.m_coordinator.GetSceneManager().getCurrentFileName().c_str());
				//g_engine.m_coordinator.GetSceneManager().SaveAndLoadLevel();
				g_engine.m_coordinator.SetGameState(true);
			}
			else //If game is running, just stop and reload old data
			{
				//Loads last iteration and pauses game
				g_engine.m_coordinator.GetSceneManager().ClearAllEntities();
				g_engine.m_coordinator.GetSceneManager().LoadLevel(g_engine.m_coordinator.GetSceneManager().getCurrentFileName().c_str());
				g_engine.m_coordinator.SetGameState(false);
			}
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("If game is paused, will save level");
			ImGui::EndTooltip();
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			//Pause/Unpause only if game is running
			if (g_engine.m_coordinator.GetGameState())
				g_engine.m_coordinator.TogglePauseState();
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Toggles Pause");
			ImGui::EndTooltip();
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			if (g_engine.m_coordinator.GetGameState())
			{
				//Loads last iteration and pauses game
				g_engine.m_coordinator.GetSceneManager().ClearAllEntities();
				g_engine.m_coordinator.GetSceneManager().LoadLevel(g_engine.m_coordinator.GetSceneManager().getCurrentFileName().c_str());
				g_engine.m_coordinator.SetGameState(false);
				g_engine.m_coordinator.SetPauseState(false);
			}
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Stops game and loads last saved state");
			ImGui::EndTooltip();
		}

		ImVec2 imageSize{ ImGui::GetContentRegionAvail() };
		ImGui::Image((void*)(intptr_t)(g_engine.m_coordinator.GetSystem<GraphicsSystem>()->getFBO()), ImVec2(imageSize.x,imageSize.y ), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}
	void ImGuiEditorViewport::Shutdown()
	{

	}
}