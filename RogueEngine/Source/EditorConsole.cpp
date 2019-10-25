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
		std::vector<float> m_Percent;
		float col_size = ImGui::GetWindowWidth() * 0.75f;
		for (const auto& iter : i)
		{
			m_Percent.push_back(iter.second);
		}
		ImGui::PlotHistogram("", m_Percent.data(), int(m_Percent.size()), 0, "Profile Time Graph", 0.0f, 100.0f, ImVec2{ col_size, 100.0f });

		for (const auto& iter : i)
		{
			m_check = iter.second;
			if (m_check > 25.0f)
			{
				ImGui::TextColored({ 1.0f,1.0f,0.0f,1.0f },"%s %.3f ms", iter.first, iter.second);
			}
			else
			{
				ImGui::Text("%s %.3f ms", iter.first, iter.second);
			}
		}
		ImGui::End();
	}

	void ImGuiConsole::Shutdown()
	{
	}

}
