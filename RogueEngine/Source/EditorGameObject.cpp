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
		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Create Empty"))
			{

			}
			if (ImGui::MenuItem("Create Empty Child"))
			{

			}
			if (ImGui::BeginMenu("2D Object"))
			{
				if (ImGui::MenuItem("Sprite"))
				{

				}
				ImGui::Separator();

				if (ImGui::MenuItem("Sprite Mask"))
				{

				}

				if (ImGui::MenuItem("Tilemap"))
				{

				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
	}
	void ImGuiGameObject::Update()
	{
	}
	void ImGuiGameObject::Shutdown()
	{
	}
}