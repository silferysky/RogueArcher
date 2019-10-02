#include "EditorConsole.h"

namespace ImGuiConsole
{
	ImGuiConsole::ImGuiConsole()
	{
	}
	ImGuiConsole::~ImGuiConsole()
	{
	}
	void ImGuiConsole::ImGuiConsoleInit()
	{
		ImGui::Begin("Console");
		if (ImGui::Button("Clear"))
		{

		}
		ImGui::SameLine();
		if(ImGui::Button("Collapse"))
		{

		}
		ImGui::SameLine();
		if (ImGui::Button("Clear On Play"))
		{

		}
		ImGui::SameLine();
		if (ImGui::Button("Clear On Build"))
		{

		}
		ImGui::SameLine();
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
		ImGui::End();
	}

}
