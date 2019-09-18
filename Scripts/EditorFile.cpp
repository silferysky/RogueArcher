#include "EditorFile.h"

namespace ImGuiEditorFile
{
	ImGuiEditorFile::ImGuiEditorFile()
	{
	}

	ImGuiEditorFile::~ImGuiEditorFile()
	{
	}

	void ImGuiEditorFile::ImGuiEditorFileMenuInit()
	{

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene"))
			{

			}
			if (ImGui::MenuItem("Open Scene"))
			{

			}
			if (ImGui::MenuItem("Save Scene"))
			{

			}
			if (ImGui::MenuItem("Save Scene As"))
			{

			}
			if (ImGui::MenuItem("New Project"))
			{

			}
			if (ImGui::MenuItem("Save Project"))
			{

			}
			if (ImGui::MenuItem("Exit"))
			{
				//gsm change state here
			}
			ImGui::EndMenu();
		}
	}
}

