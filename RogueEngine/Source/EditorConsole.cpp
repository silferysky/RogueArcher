#include "pch.h"
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


		ImGui::Separator();

		ImGui::TextDisabled("Search");
		static char bufferX[64];
		ImGui::SameLine();
		ImGui::InputText(" ", bufferX, 64);
		std::string search;
		search = bufferX;
		for (auto& i : Logger)
		{
			std::string name = i.substr(0, search.size());
			if (name == search)
			{
				ImGui::Text("%s", i.c_str());
			}
			else if (search == "")
			{
				ImGui::Text("%s", i.c_str());
			}
			
		}
		ImGui::End();
	}

	void ImGuiConsole::Shutdown()
	{
	}
}
