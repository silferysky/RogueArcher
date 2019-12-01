/* Start Header ************************************************************************/
/*!
\file           EditorGameObject.cpp
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for EditorGameObject

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "EditorGameObject.h"

namespace Rogue
{
	ImGuiGameObject::ImGuiGameObject()
	{
	}
	ImGuiGameObject::~ImGuiGameObject()
	{
	}
	void ImGuiGameObject::Init()
	{
		
	}
	void ImGuiGameObject::Update()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("GameObject"))
			{
				if (ImGui::MenuItem("Create Empty",nullptr,false,false))
				{
		
				}

				if (ImGui::MenuItem("Create Empty Child", nullptr, false, false))
				{
		
				}

				if (ImGui::BeginMenu("Load Prefab"))
				{
					for (auto& i : SceneManager::instance().GetArchetypeMap())
					{
						if(ImGui::MenuItem(i.first.c_str()))
						{
							SceneManager::instance().Clone(i.first.c_str());
						}
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("2D Object"))
				{
					if (ImGui::MenuItem("Sprite", nullptr, false, true))
					{
						for (auto& i : m_currentVector)
						{
							g_engine.m_coordinator.GetHierarchyInfo(i).m_selected = false;
						}
						SceneManager::instance().Create2DSprite();
					}
					ImGui::Separator();
		
					if (ImGui::MenuItem("Sprite Mask", nullptr, false, false))
					{
		
					}
		
					if (ImGui::MenuItem("Tilemap", nullptr, false, false))
					{
		
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
	void ImGuiGameObject::Shutdown()
	{
	}
}