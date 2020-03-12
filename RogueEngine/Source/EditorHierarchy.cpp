/* Start Header ************************************************************************/
/*!
\file           EditorHierarchy.cpp
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for EditorHierarchy

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "EditorHierarchy.h"
#include "EditorManager.h"
#include "EventDispatcher.h"
#include "EditorEvent.h"
#include "ParentEvent.h"
#include "CameraManager.h"

namespace Rogue
{
	void ImGuiEditorHierarchy::DisplayHierarchyParent(HierarchyInfo& objInfo)
	{
		if(ImGui::Selectable(objInfo.m_objectName.c_str(), objInfo.m_selected, ImGuiSelectableFlags_AllowDoubleClick))
		{

		}
		if (ImGui::IsItemClicked(0))
		{
			objInfo.m_selected = !objInfo.m_selected;
			Entity temp = objInfo.m_Entity;
			for (auto& i : m_currentVector)
			{
				if (i == temp)
				{
					if (g_engine.m_coordinator.GetHierarchyInfo(i).m_selected)
					{
						Vec2 position = g_engine.m_coordinator.GetComponent<TransformComponent>(i).GetPosition();
						glm::vec3 camerapos = glm::vec3(position.x, position.y, 0.0f);
						CameraManager::instance().SetCameraPos(camerapos);
					}
					continue;
				}

				else
				{
					g_engine.m_coordinator.GetHierarchyInfo(i).m_selected = false;
				}

			}
		}
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			ImGui::SetDragDropPayload("Hierarchy", &objInfo, sizeof(objInfo));
			ImGui::BeginTooltip();
			ImGui::Text("%s", objInfo.m_objectName.c_str());
			ImGui::EndTooltip();
			ImGui::EndDragDropSource();
		}
	
		AcceptHierarchyPayload(objInfo);
		DisplayHierarchyChildren(objInfo, 1);
	}
	
	void ImGuiEditorHierarchy::DisplayHierarchyChildren(HierarchyInfo& ent, size_t numOfParents)
	{
		for (Entity e : ent.m_children)
		{
			HierarchyInfo& childHierarchy = g_engine.m_coordinator.GetHierarchyInfo(e);
			std::ostringstream displayName;
			size_t curNumOfParents = numOfParents;
			while (curNumOfParents--)
				displayName << " ";
			displayName << "\\" << childHierarchy.m_objectName;
			if (ImGui::Selectable(displayName.str().c_str(), childHierarchy.m_selected, ImGuiSelectableFlags_AllowDoubleClick))
			{

			}

			if (ImGui::IsItemClicked(0))
			{
				childHierarchy.m_selected = !childHierarchy.m_selected;
				Entity temp = childHierarchy.m_Entity;

				for (auto& i : m_currentVector)
				{
					if (i == temp)
					{
						if (g_engine.m_coordinator.GetHierarchyInfo(i).m_selected)
						{
							Vec2 position = g_engine.m_coordinator.GetComponent<TransformComponent>(i).GetPosition();
							glm::vec3 camerapos = glm::vec3(position.x, position.y, 0.0f);
							CameraManager::instance().SetCameraPos(camerapos);
						}
						continue;
					}
					else
					{
						g_engine.m_coordinator.GetHierarchyInfo(i).m_selected = false;
					}
				}
			}
			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{
				ImGui::SetDragDropPayload("Hierarchy", &childHierarchy, sizeof(childHierarchy));
				ImGui::BeginTooltip();
				ImGui::Text("%s", childHierarchy.m_objectName.c_str());
				ImGui::EndTooltip();
				ImGui::EndDragDropSource();
			}
	
			AcceptHierarchyPayload(childHierarchy);

			//Display children of children
			//if (numOfParents < 10)
			DisplayHierarchyChildren(childHierarchy, numOfParents + 1);
		}
	}

	void ImGuiEditorHierarchy::AcceptHierarchyPayload(HierarchyInfo& objInfo)
	{
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Hierarchy"))
			{
				HierarchyInfo& hierarchyPayload = *(HierarchyInfo*)payload->Data;

				//Hierarchy must not be yourself, but would otherwise work
				if (hierarchyPayload.m_Entity != objInfo.m_Entity)
				{
					ParentSetEvent setParentEv(objInfo.m_Entity, hierarchyPayload.m_Entity);
					setParentEv.SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
					EventDispatcher::instance().AddEvent(setParentEv);
					//ReassignParentChildFlags(hierarchyPayload.m_Entity, objInfo.m_Entity);
				}
			}
			ImGui::EndDragDropTarget();
		}
	}

	bool& ImGuiEditorHierarchy::GetIsHierarchyFocused()
	{
		return m_isHierarchyFocused;
	}

	bool& ImGuiEditorHierarchy::set(bool test)
	{
		m_isHierarchyFocused = test;
		return m_isHierarchyFocused;
	}

	ImGuiEditorHierarchy::ImGuiEditorHierarchy() :
		m_currentVector{ g_engine.m_coordinator.GetActiveObjects() }
	{

	}

	ImGuiEditorHierarchy::~ImGuiEditorHierarchy()
	{
	}

	std::vector<Entity>& ImGuiEditorHierarchy::GetCurrentHierarchyVector()
	{
		return m_currentVector;
	}


	void ImGuiEditorHierarchy::Init()
	{

	}

	void ImGuiEditorHierarchy::Update()
	{
		ImGui::Begin("Hierarchy");
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("Hierarchy"))
			{
				HierarchyInfo& hierarchyPayload = *(HierarchyInfo*)payload->Data;
				//g_engine.m_coordinator.GetHierarchyInfo(i).m_children.clear();
				//m_currentVector.emplace_back(hierarchyPayload.m_Entity);
				ParentResetEvent setParentEv(hierarchyPayload.m_Entity);
				setParentEv.SetSystemReceivers((int)SystemID::id_PARENTCHILDSYSTEM);
				EventDispatcher::instance().AddEvent(setParentEv);
			}
			ImGui::EndDragDropTarget();
		}
		if (ImGui::IsWindowFocused())
		{
			set(true);
		}
		else
		{
			set(false);
		}
		
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
					g_engine.m_coordinator.GetHierarchyInfo(i).m_selected = false;
				}
				EditorCreateObjectEvent event;
				event.SetSystemReceivers((int)SystemID::id_EDITOR);
				EventDispatcher::instance().AddEvent(event);
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
			HierarchyInfo& objInfo = g_engine.m_coordinator.GetHierarchyInfo(i);
			std::string_view tagName(objInfo.m_tag.c_str(), search.size());
			std::string_view objectName(objInfo.m_objectName.c_str(), search.size());
			int pickedEntity = EditorManager::instance().GetPickedEntity();

			// If there's any picked entity
			if (pickedEntity < MAX_ENTITIES)
			{
				if (pickedEntity == i)
				{
					// Select this entity cus it's picked
					objInfo.m_selected = true;
					Entity newSelected = i;
					// Find all the other entitiies
					for (auto& hierarchyInfo : m_currentVector)
					{
						// If it's the picked entity, skip
						if (hierarchyInfo == newSelected)
							continue;
						// Unselect the rest
						else
							g_engine.m_coordinator.GetHierarchyInfo(hierarchyInfo).m_selected = false;
					}
					
					EditorManager::instance().SetPickedEntity(MAX_ENTITIES);
				}
			}

			//For filtering
			if (objInfo.m_Entity != MAX_ENTITIES && (objInfo.m_parent == MAX_ENTITIES || objInfo.m_parent == -1))
				if (tagName == search || objectName == search)
				{
					DisplayHierarchyParent(objInfo);
				}
				else if (search == "")
				{
					DisplayHierarchyParent(objInfo);
				}
		}


		ImGui::End();
	}

	void ImGuiEditorHierarchy::Shutdown()
	{
	}


}

