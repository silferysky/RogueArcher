#include "EditorConsole.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
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
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		

		std::map<const char*, float> i = g_Engine.m_coordinator.GetTimeSystem();
		std::vector<float> m_Percent;
		float col_size = ImGui::GetWindowWidth() * 0.75f;
		for (const auto& iter : i)
		{
			m_Percent.push_back(iter.second/100);
		}
		ImGui::PlotHistogram("", m_Percent.data(), int(m_Percent.size()), 0, "Profile Time Graph", 0.0f, 7.0f, ImVec2{ col_size, 100.0f });

		for (const auto& iter : i)
		{
			m_check = iter.second/100;
			if (m_check > 0.5f)
			{
				ImGui::TextColored({ 1.0f,1.0f,0.0f,1.0f },"%s %.3f ms", iter.first, iter.second/100);
			}
			else
			{
				ImGui::Text("%s %.3f ms", iter.first, iter.second/100);
			}
		}
		ImGui::End();
	}

	void ImGuiConsole::Shutdown()
	{
	}
}
