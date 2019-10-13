#include "EditorHierarchy.h"

ImGuiEditorHierarchy::ImGuiEditorHierarchy::ImGuiEditorHierarchy()
{
}

ImGuiEditorHierarchy::ImGuiEditorHierarchy::~ImGuiEditorHierarchy()
{
}

void ImGuiEditorHierarchy::ImGuiEditorHierarchy::ImGuiEditorHierarchyInit()
{
	ImGui::Begin("Hierarchy");
	if(ImGui::Button("Create"))
	{

	}
	ImGui::SameLine();
	ImGui::TextDisabled("Name");
	ImGui::SameLine();
	static char bufferX[64] = "";
	ImGui::InputText("", bufferX, 64);
	ImGui::End();
}
