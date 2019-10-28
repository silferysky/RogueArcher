#include "EditorInspector.h"

namespace Rogue
{
	ImGuiInspector::ImGuiInspector()
	{
	}

	ImGuiInspector::~ImGuiInspector()
	{
	}

	void ImGuiInspector::Init()
	{
		
	}
	void ImGuiInspector::Update()
	{
		ImGui::Begin("Inspector");
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::TextDisabled("Name");
			for (auto& i : vector)
			{
				if (i.m_selected == true)
				{
					ImGui::Text("%s", i.m_objectName.c_str());
					ImGui::Text("Scale    ");
					ImGui::SameLine();
					ImGui::PushItemWidth(75);
					ImGui::DragFloat(" ", &i.m_scaleX);
					ImGui::SameLine(0.0f, 36.0f);
					ImGui::DragFloat("  ", &i.m_scaleY);
					ImGui::Text("Rotation ");
					ImGui::SameLine();
					ImGui::DragFloat("   ", &i.m_rotateX);
					ImGui::SameLine(0.0f, 22.0f);
					ImGui::PushItemWidth(75);
					ImGui::DragFloat("    ", &i.m_rotateY);
					ImGui::Text("Translate");
					ImGui::SameLine();
					ImGui::PushItemWidth(75);
					ImGui::DragFloat("     ", &i.m_translateX);
					ImGui::SameLine();
					ImGui::PushItemWidth(75);
					ImGui::DragFloat("      ", &i.m_translateY);
				}
			}			



		}
		ImGui::PushItemWidth(50);
		if (ImGui::Button("Add Component"))
		{

		}
		ImGui::End();
	}
	void ImGuiInspector::Shutdown()
	{
	}
};

