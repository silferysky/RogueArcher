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
					for (auto& i : g_engine.m_coordinator.GetSceneManager().GetArchetypeMap())
					{
						if(ImGui::MenuItem(i.first.c_str()))
						{
							g_engine.m_coordinator.GetSceneManager().Clone(i.first.c_str());
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
							i.m_selected = false;
						}
						g_engine.m_coordinator.GetSceneManager().Create2DSprite();
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