#include "EditorComponent.h"

namespace Rogue
{
	ImGuiComponent::ImGuiComponent()
	{
	}
	ImGuiComponent::~ImGuiComponent()
	{
	}
	void ImGuiComponent::Init()
	{
		if (ImGui::BeginMenu("Component"))
		{
			if (ImGui::MenuItem("Add"))
			{

			}
			if (ImGui::BeginMenu("Mesh"))
			{
				if (ImGui::MenuItem("Mesh Filter"))
				{

				}
				if (ImGui::MenuItem("TextMesh"))
				{

				}
				ImGui::Separator();
				if (ImGui::MenuItem("Mesh Renderer"))
				{

				}
				if (ImGui::MenuItem("Skinned Mesh Renderer"))
				{

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Physics"))
			{
				if (ImGui::MenuItem("Rigid Body"))
				{

				}
				ImGui::Separator();
				if (ImGui::MenuItem("Box Collider"))
				{

				}

				if (ImGui::MenuItem("Circle Collider"))
				{

				}

				if (ImGui::MenuItem("Edge Collider"))
				{

				}

				if (ImGui::MenuItem("Polygon Collider"))
				{

				}

				ImGui::Separator();
				if (ImGui::MenuItem("Distant Joint"))
				{

				}
				if (ImGui::MenuItem("Friction Joint"))
				{

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Audio"))
			{

				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
	}
	void ImGuiComponent::Update()
	{
	}
	void ImGuiComponent::Shutdown()
	{
	}
}