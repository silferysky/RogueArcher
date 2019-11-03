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
		if (ImGui::Button("Create New Object"))
		{
			ImGui::OpenPopup("Create");
		}
		if (ImGui::BeginPopup("Create"))
		{
			if (ImGui::Selectable("2D Sprite"))
			{
				for (auto&i : m_currentVector)
				{
					i.m_selected = false;
				}
				g_engine.m_coordinator.GetSceneManager().Create2DSprite();
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Creates a default sprite with sprite and transform component");
				ImGui::EndTooltip();
			}
			if (ImGui::Selectable("Camera"))
			{
				g_engine.m_coordinator.GetSceneManager().CreateCamera();
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Creates a Camera Object");
				ImGui::EndTooltip();
			}
			ImGui::EndPopup();
		}
		ImGui::TextDisabled("Name");
		ImGui::SameLine();
		static char bufferX[64];
		ImGui::InputText(" ", bufferX, 64);
		std::string search;
		if (ImGui::Button("Search"))
		{	
			search = bufferX;
			//memset(bufferX, 0, 64);
		}
		ImGui::Separator();
		for (auto& i : m_currentVector)
		{
			if (i.m_objectName == search)
			{
				std::cout << "hi" << std::endl;
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
			else
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
		}
		ImGui::End();
	}

	void ImGuiEditorHierarchy::Shutdown()
	{
	}

}

