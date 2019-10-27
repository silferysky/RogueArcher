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
		temp data;
		ImGui::Begin("Inspector");
		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::TextDisabled("Name");
			for (auto i : vector)
			{
				if (i.m_selected == true)
				{
					ImGui::Text("%s", i.m_objectName.c_str());

					data.m_rotateX = i.m_rotateX;
					data.m_rotateY = i.m_rotateY;
					data.m_scaleX = i.m_scaleX;
					data.m_scaleY = i.m_scaleY;
					data.m_translateX = i.m_translateX;
					data.m_translateY = i.m_translateY;
				}
			}
				ImGui::Text("Scale    ");
				ImGui::SameLine();
				ImGui::PushItemWidth(50);
				ImGui::DragFloat("", &(data.m_scaleX));
				ImGui::SameLine();
				ImGui::PushItemWidth(50);
				ImGui::DragFloat("", &(data.m_scaleY));
				ImGui::Text("Rotation ");
				ImGui::SameLine();
				ImGui::DragFloat("", &(data.m_rotateX));
				ImGui::SameLine();
				ImGui::PushItemWidth(50);
				ImGui::DragFloat("", &(data.m_rotateY));
				ImGui::Text("Translate");
				ImGui::SameLine();
				ImGui::PushItemWidth(50);
				ImGui::DragFloat("", &(data.m_translateX));
				ImGui::SameLine();
				ImGui::PushItemWidth(50);
				ImGui::DragFloat("", &(data.m_translateY));

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

