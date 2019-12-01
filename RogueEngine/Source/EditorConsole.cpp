/* Start Header ************************************************************************/
/*!
\file           EditorConsole.cpp
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function for editorconsole

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "EditorConsole.h"
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
		std::string_view search(bufferX);
		for (auto& i : Logger)
		{
			std::string_view name(i.c_str(), search.size());
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
