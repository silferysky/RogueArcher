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
		

		std::map<const char*, float> timeSystem = g_engine.m_coordinator.GetSystemTimes();

		float col_size = ImGui::GetWindowWidth() * 0.75f;

		auto iter = timeSystem.begin();

		ImGui::PlotHistogram("", &iter->second, int(timeSystem.size()), 0, "Profile Time Graph", 0.0f, 7.0f, ImVec2{ col_size, 100.0f });

		for (const auto& iter : timeSystem)
		{
			m_check = iter.second;
			if (m_check > 50.0f)
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
