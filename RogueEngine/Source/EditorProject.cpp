#include "EditorProject.h"

namespace Rogue
{
	ImGuiProject::ImGuiProject()
	{
	}

	ImGuiProject::~ImGuiProject()
	{
	}

	void ImGuiProject::Init()
	{
		

	}

	void ImGuiProject::Update()
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

	void ImGuiProject::Shutdown()
	{
	}
}

