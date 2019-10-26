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
				HierarchyInfo temp;
				temp.count = iterator;
				temp.m_objectName = "Game Object ";
				temp.m_objectName += temp.count + '0';
				m_currentActiveObjects.push_back(temp);
				++iterator;
			}
			if (ImGui::Selectable("Camera"))
			{

			}
			ImGui::EndPopup();
		}
		ImGui::SameLine();
		ImGui::TextDisabled("Name");
		ImGui::SameLine();
		static char bufferX[64] = "";
		ImGui::InputText("", bufferX, 64);
		ImGui::Separator();
		static int j = -1;
		for (auto& i : m_currentActiveObjects)
		{
			if (ImGui::Selectable(i.m_objectName.c_str(), i.m_selected, ImGuiSelectableFlags_AllowDoubleClick))
			{
				
			}
		}
		ImGui::End();
	}

	void ImGuiEditorHierarchy::Shutdown()
	{
	}
}

