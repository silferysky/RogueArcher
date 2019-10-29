#include "EditorHierarchy.h"

namespace Rogue
{

	ImGuiEditorHierarchy::ImGuiEditorHierarchy()
	{
	}

	ImGuiEditorHierarchy::~ImGuiEditorHierarchy()
	{
	}

	void ImGuiEditorHierarchy::Init()
	{
		
	}

	void ImGuiEditorHierarchy::Update()
	{
		ImGui::Begin("Hierarchy");
		if (ImGui::Button("Create"))
		{
			ImGui::OpenPopup("Create");
		}
		if (ImGui::BeginPopup("Create"))
		{
			if (ImGui::Selectable("2D Sprite"))
			{
				g_engine.m_coordinator.GetSceneManager().Create2DSprite();
			}
			if (ImGui::Selectable("Camera"))
			{
				g_engine.m_coordinator.GetSceneManager().CreateCamera();
			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::TextDisabled("Name");
		ImGui::SameLine();
		static char bufferX[64] = "";
		ImGui::InputText("", bufferX, 64);
		ImGui::Separator();
		for (auto& i : m_currentVector)
		{
			if (ImGui::Selectable(i.m_objectName.c_str(), i.m_selected, ImGuiSelectableFlags_AllowDoubleClick))
			{
				if (ImGui::IsMouseClicked(0))
				{
					i.m_selected = !i.m_selected;
					int temp = i.m_Entity;
					for (auto& i : m_currentVector)
					{
						if (i.m_Entity == temp)
							continue;
						else
						{
							i.m_selected = false;
						}
					}
				}
			}
		}
		ImGui::End();
	}

	void ImGuiEditorHierarchy::Shutdown()
	{
	}
}

