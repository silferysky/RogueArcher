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
		ImGui::Separator();
		//if (ImGui::Button("Error Pause"))
		//{
		//
		//}
		//ImGui::SameLine();
		//if (ImGui::BeginMenu("Editor"))
		//{
		//	if (ImGui::MenuItem("Player Logging"))
		//	{
		//
		//	}
		//	ImGui::Separator();
		//	if (ImGui::MenuItem("Editor"))
		//	{
		//
		//	}
		//	ImGui::EndMenu();
		//}

		ImGui::End();
	}

	void ImGuiConsole::Shutdown()
	{
	}
}
