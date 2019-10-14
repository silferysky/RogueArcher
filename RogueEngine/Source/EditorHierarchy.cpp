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
		ImGui::Begin("Hierarchy");
		if (ImGui::Button("Create"))
		{

		}
		ImGui::SameLine();
		ImGui::TextDisabled("Name");
		ImGui::SameLine();
		static char bufferX[64] = "";
		ImGui::InputText("", bufferX, 64);
		ImGui::End();
	}

	void ImGuiEditorHierarchy::Update()
	{
	}

	void ImGuiEditorHierarchy::Shutdown()
	{
	}
}

