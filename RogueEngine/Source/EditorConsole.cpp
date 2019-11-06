#include "EditorConsole.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "glew.h"
#include "glfw3.h"
#include "Logger.h"

namespace Rogue
{
	ImGuiConsole::ImGuiConsole()
	{}

	void ImGuiConsole::Init()
	{}

	void ImGuiConsole::Update()
	{
		auto& Logger = Logger::instance().GetConsole();
		ImGui::Begin("Console");
		if (ImGui::Button("Clear"))
		{
			for (auto& i : Logger)
			{
				i.clear();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Collapse"))
		{
		
		}
		ImGui::Separator();

		
		
		for (auto& i : Logger)
		{
			ImGui::Text("%s", i.c_str());
		}
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
