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

		ImVec2 imageSize{ ImGui::GetContentRegionAvail() };
		ImGui::Image((void*)(intptr_t)(g_engine.m_coordinator.GetSystem<GraphicsSystem>()->getFBO()), ImVec2(imageSize.x,imageSize.y ), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}
	void ImGuiEditorViewport::Shutdown()
	{
	}
}