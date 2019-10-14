#include "EditorEdit.h"

namespace ImGuiEditorEdit
{
	ImGuiEditorEdit::ImGuiEditorEdit()
	{
	}
	ImGuiEditorEdit::~ImGuiEditorEdit()
	{
	}
	void ImGuiEditorEdit::ImGuiEditorEditInit()
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
	}
	void ImGuiEditorEdit::Update()
	{
	}
	void ImGuiEditorEdit::Shutdown()
	{
	}
}