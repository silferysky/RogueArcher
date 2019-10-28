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
				if (ImGui::BeginMenu("2D Object"))
				{
					if (ImGui::MenuItem("Sprite", nullptr, false, false))
					{
		
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