#include "EditorConsole.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "glew.h"
#include "glfw3.h"

namespace Rogue
{
	ImGuiConsole::ImGuiConsole()
	{}

	void ImGuiConsole::Init()
	{}

	void ImGuiConsole::Update()
	{
		ImGui::Begin("Console");
		if (ImGui::Button("Clear"))
		{

		}
		ImGui::SameLine();
		if (ImGui::Button("Collapse"))
		{

		}
		if (ImGui::Button("Error Pause"))
		{

		}
		ImGui::SameLine();
		if (ImGui::BeginMenu("Editor"))
		{
			if (ImGui::MenuItem("Player Logging"))
			{

			}
			ImGui::Separator();
			if (ImGui::MenuItem("Editor"))
			{

			}
			ImGui::EndMenu();
		}
		ImGui::Text("Application FPS (%.1f FPS)", ImGui::GetIO().Framerate);
		
		std::map<const char*, float> i = gEngine.m_coordinator.GetTimeSystem();
		for (const auto& iter : i)
		{
			ImGui::Text("%s %.3f ms",iter.first,iter.second);
		}

		ImGui::End();
	}

	void ImGuiConsole::Shutdown()
	{
	}

}
