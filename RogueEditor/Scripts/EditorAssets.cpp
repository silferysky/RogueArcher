#include "EditorAssets.h"

namespace ImGuiAssets
{
	ImGuiAssets::ImGuiAssets()
	{
	}
	ImGuiAssets::~ImGuiAssets()
	{
	}
	void ImGuiAssets::ImGuiAssetsInit()
	{
		if (ImGui::BeginMenu("Assets"))
		{
			if (ImGui::BeginMenu("Create"))
			{
				if (ImGui::MenuItem("Folder"))
				{

				}

				if (ImGui::MenuItem("PreFab"))
				{

				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Open", "CTRL + O", false, false))
			{

			}
			if (ImGui::MenuItem("Delete", "", false, false))
			{

			}
			if (ImGui::MenuItem("Rename", "", false, false))
			{

			}
			ImGui::EndMenu();
		}
	}
}