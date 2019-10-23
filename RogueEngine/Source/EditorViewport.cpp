#include "EditorViewport.h"

namespace Rogue
{
	ImGuiEditorViewport::ImGuiEditorViewport()
	{
	}
	ImGuiEditorViewport::~ImGuiEditorViewport()
	{
	}
	void ImGuiEditorViewport::Init()
	{

	}
	void ImGuiEditorViewport::Update()
	{
		ImGui::Begin("Viewport");
		if (ImGui::Button("Play"))
		{

		}
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{

		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{

		}
		ImGui::End();
	}
	void ImGuiEditorViewport::Shutdown()
	{
	}
}