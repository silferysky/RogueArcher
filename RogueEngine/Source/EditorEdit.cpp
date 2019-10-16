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
				if (ImGui::MenuItem("Undo", "CTRL+Z"))
				{

				}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
				{

				}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X"))
				{

				}
				if (ImGui::MenuItem("Copy", "CTRL+C"))
				{

				}
				if (ImGui::MenuItem("Paste", "CTRL+V"))
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
				if (ImGui::MenuItem("Delete", "", false, false))
				{

				}
				if (ImGui::MenuItem("Rename", "", false, false))
				{

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