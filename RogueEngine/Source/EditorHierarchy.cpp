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
				std::stringstream strstream;
				std::string str = "Game Object ";
				strstream << str << iterator;
				temp.m_Entity = iterator;
				temp.m_objectName = strstream.str();
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
				if (ImGui::IsMouseClicked(0))
				{
					i.m_selected = !i.m_selected;
					int temp = i.m_Entity;
					for (auto& i : m_currentActiveObjects)
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
	std::vector<HierarchyInfo>& ImGuiEditorHierarchy::m_getActiveObjects()
	{
		return m_currentActiveObjects;
	}
}

