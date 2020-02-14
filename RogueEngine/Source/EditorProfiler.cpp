/* Start Header ************************************************************************/
/*!
\file           EditorProfiler.cpp
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for EditorProfiler

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "EditorProfiler.h"

namespace Rogue
{
	void ImGuiProfiler::Init()
	{
	}
	void ImGuiProfiler::Update()
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


		// Time is in microseconds
		const std::map<const char*, float>& timeSystem = g_engine.m_coordinator.GetSystemTimes();
		std::vector<float> vecTimeSystem; // For histogram

		float col_size = ImGui::GetWindowWidth() * 0.75f;
		float col_height = ImGui::GetWindowHeight() * 0.25f;

		auto iter = timeSystem.begin();
		for (const auto& iter : timeSystem)
		{
			float systemTime = iter.second / Timer::s_microsecPerSec; // Convert systemTime from microsec to seconds
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
	void ImGuiProfiler::Shutdown()
	{
	}
}