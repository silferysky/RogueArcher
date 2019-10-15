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
	void ImGuiEditorEdit::Shutdown()
	{
	}
}