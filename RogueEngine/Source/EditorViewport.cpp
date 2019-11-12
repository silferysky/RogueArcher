#include "Precompiled.h"
#include "EditorViewport.h"
#include "Main.h"

namespace Rogue
{
	void ImGuiEditorViewport::Init()
	{}

	void ImGuiEditorViewport::Update()
	{
		ImGui::Begin("Viewport");
		ImGuiStyle& style = ImGui::GetStyle();
		if (ImGui::Button("Play"))
		{
			//If game is not running, save level, and set it to running
			if (!g_engine.m_coordinator.GetGameState())
			{
				SceneManager::instance().SaveLevel(SceneManager::instance().getCurrentFileName().c_str());
				//SceneManager::instance().SaveAndLoadLevel();
				g_engine.m_coordinator.SetGameState(true);
				//ShowCursor(false);
			}
			else //If game is running, just stop and reload old data
			{
				//Loads last iteration and pauses game
				SceneManager::instance().ClearAllEntities();
				SceneManager::instance().LoadLevel(SceneManager::instance().getCurrentFileName().c_str());
				g_engine.m_coordinator.SetGameState(false);
				g_engine.m_coordinator.SetPauseState(false);
				//ShowCursor(true);
			}
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("If game is paused, will save level");
			ImGui::EndTooltip();
		}
		ImGui::SameLine();
		if (!g_engine.m_coordinator.GetGameState())
		{
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f,0.0f,0.0f,0.4f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.8f,0.0f,0.0f,1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.9f,0.0f,0.0f,1.0f });
		}
		else if(!g_engine.m_coordinator.GetPauseState())
		{
			ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_Button]);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_ButtonHovered]);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, style.Colors[ImGuiCol_ButtonActive]);
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, {0.3f, 0.3f, 0.3f, 0.3f});
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.8f, 0.8f, 0.8f, 0.8f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f, 0.8f, 0.8f, 0.8f });
		}
		if (ImGui::Button("Pause"))
		{
			//Pause/Unpause only if game is running
			if (g_engine.m_coordinator.GetGameState())
			{
				g_engine.m_coordinator.TogglePauseState();
				//ShowCursor(true);
			}
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Toggles Pause");
			ImGui::EndTooltip();
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		if (!g_engine.m_coordinator.GetGameState())
		{
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f,0.0f,0.0f,0.4f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.8f,0.0f,0.0f,1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.9f,0.0f,0.0f,1.0f });
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_Button]);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_ButtonHovered]);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, style.Colors[ImGuiCol_ButtonActive]);
		}
		if (ImGui::Button("Stop"))
		{
			if (g_engine.m_coordinator.GetGameState())
			{
				//Loads last iteration and pauses game
				SceneManager::instance().ClearAllEntities();
				SceneManager::instance().LoadLevel(SceneManager::instance().getCurrentFileName().c_str());
				g_engine.m_coordinator.SetGameState(false);
				g_engine.m_coordinator.SetPauseState(false);
			}
		}
		ImGui::PopStyleColor(3);
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Stops game and loads last saved state");
			ImGui::EndTooltip();
		}
		ImGui::SameLine();
		int m_Frames = (int)g_engine.m_coordinator.GetStepFrames();
		ImGui::PushItemWidth(75);
		ImGui::SliderInt("Step Count", &m_Frames, 1, 60);
		g_engine.m_coordinator.SetStepFrames(m_Frames);
		ImGui::SameLine();
		if (ImGui::Button("Step"))
		{
			g_engine.m_coordinator.StepOnce();
		}

		ImVec2 imageSize = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)(intptr_t)(g_engine.m_coordinator.GetSystem<GraphicsSystem>()->getFBO()), ImVec2(imageSize.x,imageSize.y ), ImVec2(0, 1), ImVec2(1, 0));


		ImVec2 mousePos = ImGui::GetMousePos();
		int width = g_engine.GetEngineWindowWidth();
		int height = g_engine.GetEngineWindowHeight();

		mousePos.x = (mousePos.x - ImGui::GetCursorScreenPos().x) * width / imageSize.x;
		mousePos.y = (mousePos.y - ImGui::GetCursorScreenPos().y) * height / -imageSize.y;

		// TODO: Move to cursor system to get viewport screen pos from here

		float x = (2.0f * mousePos.x) / width - 1.0f;
		float y = 1.0f - (2.0f * mousePos.y) / height;
		float z = 1.0f;

		glm::vec3 rayNDC = glm::vec3(x, y, z);

		// convert from ndc to viewport coordinates if editor is on?

		glm::vec4 rayClip = glm::vec4(rayNDC.x, rayNDC.y, -1.0f, 1.0f);

		glm::vec4 rayEye = glm::inverse(g_engine.GetProjMat()) * rayClip;

		glm::mat4 viewMat = g_engine.m_coordinator.GetSystem<CameraSystem>()->GetViewMatrix(1.0f);

		glm::vec4 rayWorld4D = glm::inverse(viewMat) * rayEye;

		g_engine.SetViewportCursor(ImVec2(rayWorld4D.x, -rayWorld4D.y));

		ImGui::End();
	}
	void ImGuiEditorViewport::Shutdown()
	{

	}
}