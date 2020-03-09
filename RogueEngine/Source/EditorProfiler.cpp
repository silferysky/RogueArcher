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
#include "SystemList.h"
#include "EditorSettings.h"
#include "REMath.h"

namespace Rogue
{
	ImGuiProfiler::ImGuiProfiler() :
		m_timeSystemRef(g_engine.m_coordinator.GetSystemTimes()),
		m_profileAge{ 0.0f },
		m_profileInterval{ 0.5f }
	{}

	void ImGuiProfiler::Init()
	{
		m_vecTimeSystem.reserve(static_cast<int>(SystemID::id_LASTSYS));

		// Set interval to 2 seconds
		m_profileInterval = 0.5f;
		m_profileAge = 0.0f;
	}
	void ImGuiProfiler::Update()
	{
		ImGui::Begin("Profiler");
		ImGui::TextWrapped("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::NewLine();
		ImGui::TextWrapped("%d SPS", static_cast<int>(1 / g_fixedDeltaTime + 1.0f));
		
		float col_size = ImGui::GetWindowWidth() * 0.75f;
		float col_height = ImGui::GetWindowHeight() * 0.25f;

		if (m_profileAge >= m_profileInterval)
		{	
			m_profileAge = 0.0f;
			m_vecTimeSystem.clear();

			float highestDt = 0.0f;
			std::string highestSystem;

			for (const auto& iter : m_timeSystemRef)
			{
				float systemTime = iter.second;
				if (systemTime > highestDt)
				{
					highestDt = systemTime;
					highestSystem = std::string(iter.first);
				}
			}

			for (const auto& iter : m_timeSystemRef)
			{
				float systemTime = iter.second;
				systemTime /= Timer::s_microsecPerSec; // Convert systemTime from microsec to seconds
				systemTime = systemTime / g_deltaTime * 100.0f;
				//std::cout << (g_engine.m_coordinator.getcurrentState()) << std::endl;

				if (std::string(iter.first) == highestSystem)
				{
					switch (ImGuiEditorSettings::instance().getcurrentState())
					{
					case Style::Classic:
					{
						ImGui::TextColored({ 1.0f,1.0f,0.0f,1.0f }, "%s %.2f %%", iter.first, systemTime);
						break;
					}
					case Style::Dark:
					{
						ImGui::TextColored({ 1.0f,1.0f,0.0f,1.0f }, "%s %.2f %%", iter.first, systemTime);
						break;
					}
					case Style::Light:
					{
						ImGui::TextColored({ 0.0f,0.0f,0.0f,1.0f }, "%s %.2f %%", iter.first, systemTime);
						break;
					}
					default:
						break;
					}
				}
				else
				{
					ImGui::Text("%s %.2f %%", iter.first, systemTime);
				}

				// Time is in milliseconds
				m_vecTimeSystem.emplace_back(systemTime);
			}


		}
		else
		{
			float highestDt = 0.0f;
			std::string highestSystem;

			for (const auto& iter : m_timeSystemRef)
			{
				float systemTime = iter.second;
				if (systemTime > highestDt)
				{
					highestDt = systemTime;
					highestSystem = std::string(iter.first);
				}
			}

			for (const auto& iter : m_timeSystemRef)
			{
				float systemTime = iter.second;
				systemTime /= Timer::s_microsecPerSec; // Convert systemTime from microsec to seconds
				
				if(g_deltaTime)
					systemTime = systemTime / g_deltaTime * 100.0f;
				//std::cout << (g_engine.m_coordinator.getcurrentState()) << std::endl;

				if (std::string(iter.first) == highestSystem)
				{
					Style style = ImGuiEditorSettings::instance().getcurrentState();
					switch (style)
					{
					case Style::Classic:
					{
						ImGui::TextColored({ 1.0f,1.0f,0.0f,1.0f }, "%s %.2f %%", iter.first, systemTime);
						break;
					}
					case Style::Dark:
					{
						ImGui::TextColored({ 1.0f,1.0f,0.0f,1.0f }, "%s %.2f %%", iter.first, systemTime);
						break;
					}
					case Style::Light:
					{
						ImGui::TextColored({ 0.0f,0.0f,0.0f,1.0f }, "%s %.2f %%", iter.first, systemTime);
						break;
					}
					default:
						break;
					}
				}
				else
				{
					ImGui::Text("%s %.2f %%", iter.first, systemTime);
				}
			}
		}
		ImGui::PlotHistogram("", m_vecTimeSystem.data(), int(m_vecTimeSystem.size()), 0, "Profile Time Graph", 0.0f, 100.0f, ImVec2{ col_size, col_height });

		ImGui::End();

		m_profileAge += g_deltaTime;
	}

	void ImGuiProfiler::Shutdown()
	{
	}
}