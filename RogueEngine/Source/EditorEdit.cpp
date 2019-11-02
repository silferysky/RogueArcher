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
						if (i.m_selected == true)
						{
							g_engine.m_coordinator.GetSceneManager().DeleteActiveEntity(i.m_Entity);
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