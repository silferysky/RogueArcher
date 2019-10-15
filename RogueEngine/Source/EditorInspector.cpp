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
		ImGui::Begin("Inspector");
		if (ImGui::CollapsingHeader("Transform"))
		{
			static char bufferX[64] = "";
			static char bufferY[64] = "";
			static char bufferZ[64] = "";
			ImGui::Text("Position        ");
			ImGui::SameLine();
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::InputText("",bufferX,64);
			ImGui::SameLine();
			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::InputText("", bufferY, 64);
			ImGui::SameLine();
			ImGui::Text("Z");
			ImGui::SameLine();
			ImGui::InputText("", bufferZ, 64);
			ImGui::Text("Rotation        ");
			ImGui::SameLine();
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::InputText("", bufferX, 64);
			ImGui::SameLine();
			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::InputText("", bufferY, 64);
			ImGui::SameLine();
			ImGui::Text("Z");
			ImGui::SameLine();
			ImGui::InputText("", bufferZ, 64);
			ImGui::Text("Scale           ");
			ImGui::SameLine();
			ImGui::Text("X");
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::InputText("", bufferX, 64);
			ImGui::SameLine();
			ImGui::Text("Y");
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			ImGui::InputText("", bufferY, 64);
			ImGui::SameLine();
			ImGui::Text("Z");
			ImGui::SameLine();
			ImGui::InputText("", bufferZ, 64);
		}
		ImGui::PushItemWidth(50);
		if (ImGui::Button("Add Component"))
		{

		}
		ImGui::End();
	}
	void ImGuiInspector::Update()
	{
	}
	void ImGuiInspector::Shutdown()
	{
	}
};

