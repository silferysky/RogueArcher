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