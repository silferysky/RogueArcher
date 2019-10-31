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
		ImGui::Begin("Profiler");
		/*if (ImGui::Button("Clear"))
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
		}*/
		ImGui::TextWrapped("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::TextWrapped("%.3f ms/frame (%.1f FPS)", static_cast<double>(g_deltaTime * Timer::s_millisecondsPerSecond), 1 / g_deltaTime);


		// Time is in microseconds
		std::map<const char*, float> timeSystem = g_engine.m_coordinator.GetSystemTimes();
		std::vector<float> vecTimeSystem; // For histogram

		float col_size = ImGui::GetWindowWidth() * 0.75f;
		float col_height = ImGui::GetWindowHeight() * 0.25f;
		float dtInMilliseconds = g_deltaTime / Timer::s_millisecondsPerSecond;

		
		auto iter = timeSystem.begin();
		for (const auto& iter : timeSystem)
		{
			float systemTime = iter.second / Timer::s_microsecondsPerSecond;
			systemTime = systemTime / g_deltaTime * 100.0f;

			if (systemTime > 20.0f)
			{
				ImGui::TextColored({ 1.0f,1.0f,0.0f,1.0f }, "%s %.2f %%", iter.first, systemTime);
			}
			else
			{
				ImGui::Text("%s %.2f %%", iter.first, systemTime);
			}

			// Time is in milliseconds
			vecTimeSystem.push_back(systemTime);
		}

		ImGui::PlotHistogram("", vecTimeSystem.data(), int(vecTimeSystem.size()), 0, "Profile Time Graph", 0.0f, 100.0f, ImVec2{ col_size, col_height });

		ImGui::End();
	}

	void ImGuiConsole::Shutdown()
	{
	}
}
