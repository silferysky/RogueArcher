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
			for (auto& i : activeObjects)
			{
				if (i.m_selected == true)
				{
					Vec2 Scale = g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity).getScale();
					Vec2 Position = g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity).getPosition();
					float Rotation = g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity).getRotation();
					ImGui::Text("%s", i.m_objectName.c_str());
					ImGui::Text("Scale    ");
					ImGui::SameLine();
					ImGui::PushItemWidth(75);
					ImGui::DragFloat(" ", &Scale.x);
					ImGui::SameLine(0.0f, 36.0f);
					ImGui::DragFloat("  ", &Scale.y);
					g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity).setScale(Scale);

					ImGui::Text("Rotation ");
					ImGui::SameLine();
					ImGui::DragFloat("   ", &Rotation);
					g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity).setRotation(Rotation);
					ImGui::Text("Translate");
					ImGui::SameLine();
					ImGui::PushItemWidth(75);
					ImGui::DragFloat("     ", &Position.x);
					ImGui::SameLine();
					ImGui::PushItemWidth(75);
					ImGui::DragFloat("      ", &Position.y);
					g_engine.m_coordinator.GetComponent<TransformComponent>(i.m_Entity).setPosition(Position);
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

