#include "EditorFile.h"

namespace Rogue
{
	ImGuiEditorFile::ImGuiEditorFile()
	{
	}

	ImGuiEditorFile::~ImGuiEditorFile()
	{
	}

	void ImGuiEditorFile::Init()
	{

		
	}
	void ImGuiEditorFile::Update()
	{
		if (ImGui::BeginMainMenuBar())
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
			ImGui::EndMainMenuBar();
		}
		
	}
	void ImGuiEditorFile::Shutdown()
	{
	}
}

