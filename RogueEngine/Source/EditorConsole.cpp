#include "EditorConsole.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "glew.h"
#include "glfw3.h"

namespace Rogue
{
	ImGuiConsole::ImGuiConsole()
	{
	}

	ImGuiConsole::~ImGuiConsole()
	{
	}
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
		
		std::map<const char*, float> TimerMap = gEngine.m_coordinator.GetTimeSystem();
		for (const auto& iter : TimerMap)
		{
			ImGui::Text("%c time : %.1f seconds", iter.first, iter.second);
		}

		ImGui::End();
	}

	void ImGuiConsole::Shutdown()
	{
	}

}
