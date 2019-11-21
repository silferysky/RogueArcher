#include "Precompiled.h"
#include "EditorHierarchy.h"
#include "EditorManager.h"

namespace Rogue
{

	ImGuiEditorHierarchy::ImGuiEditorHierarchy() :
		m_currentVector{ g_engine.m_coordinator.GetActiveObjects() }
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
				SceneManager::instance().Create2DSprite();
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::Text("Creates a default sprite with sprite and transform component");
				ImGui::EndTooltip();
			}
			if (ImGui::Selectable("Camera"))
			{
				SceneManager::instance().CreateCamera();
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
		std::string_view search(bufferX);
		for (auto& i : m_currentVector)
		{
			std::string_view tagName(i.m_tag.c_str(), search.size());
			std::string_view objectName(i.m_objectName.c_str(), search.size());
			int pickedEntity = EditorManager::instance().GetPickedEntity();

			// If there's any picked entity
			if (pickedEntity >= 0)
			{
				if (pickedEntity == i.m_Entity)
				{
					// Select this entity cus it's picked
					i.m_selected = true;
					Entity newSelected = i.m_Entity;

					// Find all the other entitiies
					for (auto& hierarchyInfo : m_currentVector)
					{
						// If it's the picked entity, skip
						if (hierarchyInfo.m_Entity == newSelected)
							continue;
						// Unselect the rest
						else
							hierarchyInfo.m_selected = false;
					}
				}
			}
			if (tagName == search || objectName == search)
			{
				if (ImGui::Selectable(i.m_objectName.c_str(), i.m_selected, ImGuiSelectableFlags_AllowDoubleClick))
				{
					if (ImGui::IsMouseClicked(0))
					{
						i.m_selected = !i.m_selected;
						Entity temp = i.m_Entity;
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
			else if (search == "")
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

