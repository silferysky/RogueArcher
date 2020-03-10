/* Start Header ************************************************************************/
/*!
\file           EditorEdit.cpp
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function for EditorEdit

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "EditorEdit.h"

namespace Rogue
{
	ImGuiEditorEdit::ImGuiEditorEdit()
	{
	}
	ImGuiEditorEdit::~ImGuiEditorEdit()
	{
	}
	void ImGuiEditorEdit::Init()
	{
	
	}
	void ImGuiEditorEdit::Update()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z",false,false))
				{

				}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
				{

				}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X",false,false))
				{

				}
				if (ImGui::MenuItem("Copy", "CTRL+C",false,false))
				{

				}
				if (ImGui::MenuItem("Paste", "CTRL+V",false,false))
				{

				}
				ImGui::EndMenu();
			}
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
				if (ImGui::MenuItem("Delete", "", false, true))
				{
					for (auto& i : m_currentVector)
					{
						if (g_engine.m_coordinator.GetHierarchyInfo(i).m_selected == true)
						{
							//SceneManager::instance().DeleteActiveEntity(i);
							g_engine.m_coordinator.DestroyEntity(i);
						}
					}
				}
				ImGui::EndMenu();
			}
			
			ImGui::EndMainMenuBar();
		}

	}
	void ImGuiEditorEdit::Shutdown()
	{
	}
}