#include "EditorProject.h"

ImGuiProject::ImGuiProject::ImGuiProject()
{
}

ImGuiProject::ImGuiProject::~ImGuiProject()
{
}

void ImGuiProject::ImGuiProject::ImGuiProjectInit()
{
	ImGui::Begin("Project");
	if (ImGui::CollapsingHeader("Folder Hierachy"))
	{

	}
	ImGui::End();
	ImGui::Begin("File");
	if (ImGui::CollapsingHeader("File Display"))
	{

	}
	ImGui::End();

}
