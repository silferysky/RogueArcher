#include "pch.h"
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

	}
	void ImGuiComponent::Update()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Component"))
			{
				if (ImGui::MenuItem("Add",nullptr,false,false))
				{

				}
				if (ImGui::BeginMenu("Mesh"))
				{
					if (ImGui::MenuItem("Mesh Filter", nullptr, false, false))
					{

					}
					if (ImGui::MenuItem("TextMesh", nullptr, false, false))
					{

					}
					ImGui::Separator();
					if (ImGui::MenuItem("Mesh Renderer", nullptr, false, false))
					{

					}
					if (ImGui::MenuItem("Skinned Mesh Renderer", nullptr, false, false))
					{

					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Physics"))
				{
					if (ImGui::MenuItem("Rigid Body", nullptr, false, false))
					{

					}
					ImGui::Separator();
					if (ImGui::MenuItem("Box Collider", nullptr, false, false))
					{

					}

					if (ImGui::MenuItem("Circle Collider", nullptr, false, false))
					{

					}

					if (ImGui::MenuItem("Edge Collider", nullptr, false, false))
					{

					}

					if (ImGui::MenuItem("Polygon Collider", nullptr, false, false))
					{

					}

					ImGui::Separator();
					if (ImGui::MenuItem("Distant Joint", nullptr, false, false))
					{

					}
					if (ImGui::MenuItem("Friction Joint", nullptr, false, false))
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
			ImGui::EndMainMenuBar();
		}
		
	}
	void ImGuiComponent::Shutdown()
	{
	}
}