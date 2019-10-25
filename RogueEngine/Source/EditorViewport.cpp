#include "EditorViewport.h"
#include "Main.h"

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

		ImVec2 imageSize{ ImGui::GetWindowSize() };
		ImGui::Image((void*)(intptr_t)(g_Engine.m_coordinator.GetSystem<GraphicsSystem>()->getFBO()), imageSize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}
	void ImGuiEditorViewport::Shutdown()
	{
	}
}