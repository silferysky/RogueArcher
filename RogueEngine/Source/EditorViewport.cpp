#include "Precompiled.h"
#include "EditorViewport.h"
#include "Main.h"
#include "PickingManager.h"
#include "CameraManager.h"
#include "REMath.h"

namespace Rogue
{
	ImGuiEditorViewport::ImGuiEditorViewport() :
		m_currentVector{ g_engine.m_coordinator.GetActiveObjects() }, m_CurrentGizmoOperation{ ImGuizmo::TRANSLATE }
	{
	}
	void ImGuiEditorViewport::Init()
	{}

	void ImGuiEditorViewport::Update()
	{
		ImGui::Begin("Viewport");
		ImGuiStyle& style = ImGui::GetStyle();
		if (ImGui::Button("Play"))
		{
			//If game is not running, save level, and set it to running
			if (!g_engine.m_coordinator.GetGameState())
			{
				SceneManager::instance().SaveLevel(SceneManager::instance().getCurrentFileName().c_str());
				//SceneManager::instance().SaveAndLoadLevel();
				g_engine.m_coordinator.SetGameState(true);
				//ShowCursor(false);
			}
			else //If game is running, just stop and reload old data
			{
				//Loads last iteration and pauses game
				SceneManager::instance().ReloadLevel();
				//ShowCursor(true);
			}
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("If game is paused, will save level");
			ImGui::EndTooltip();
		}
		ImGui::SameLine();
		if (!g_engine.m_coordinator.GetGameState())
		{
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f,0.0f,0.0f,0.4f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.8f,0.0f,0.0f,1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.9f,0.0f,0.0f,1.0f });
		}
		else if(!g_engine.m_coordinator.GetPauseState())
		{
			ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_Button]);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_ButtonHovered]);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, style.Colors[ImGuiCol_ButtonActive]);
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, {0.3f, 0.3f, 0.3f, 0.3f});
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.8f, 0.8f, 0.8f, 0.8f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.8f, 0.8f, 0.8f, 0.8f });
		}
		if (ImGui::Button("Pause"))
		{
			//Pause/Unpause only if game is running
			if (g_engine.m_coordinator.GetGameState())
			{
				g_engine.m_coordinator.TogglePauseState();
				//ShowCursor(true);
			}
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Toggles Pause");
			ImGui::EndTooltip();
		}
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		if (!g_engine.m_coordinator.GetGameState())
		{
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.8f,0.0f,0.0f,0.4f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.8f,0.0f,0.0f,1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.9f,0.0f,0.0f,1.0f });
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_Button]);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_ButtonHovered]);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, style.Colors[ImGuiCol_ButtonActive]);
		}
		if (ImGui::Button("Stop"))
		{
			if (g_engine.m_coordinator.GetGameState())
			{
				//Loads last iteration and pauses game
				SceneManager::instance().ReloadLevel(); 
			}
		}
		ImGui::PopStyleColor(3);
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Stops game and loads last saved state");
			ImGui::EndTooltip();
		}
		ImGui::SameLine();
		int m_Frames = (int)g_engine.m_coordinator.GetStepFrames();
		ImGui::PushItemWidth(75);
		ImGui::SliderInt("Step Count", &m_Frames, 1, 60);
		g_engine.m_coordinator.SetStepFrames(m_Frames);
		ImGui::SameLine();
		if (ImGui::Button("Step"))
		{
			g_engine.m_coordinator.StepOnce();
		}

		ImGui::SameLine();
		if (ImGui::RadioButton("Translate", m_CurrentGizmoOperation == ImGuizmo::TRANSLATE))
			m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", m_CurrentGizmoOperation == ImGuizmo::ROTATE))
			m_CurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", m_CurrentGizmoOperation == ImGuizmo::SCALE))
			m_CurrentGizmoOperation = ImGuizmo::SCALE;
		for (auto& i : m_currentVector)
		{
			HierarchyInfo& objInfo = g_engine.m_coordinator.GetHierarchyInfo(i);
			if (objInfo.m_selected == true)
				ShowGizmo(i);
		}

		ImVec2 imageSize = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)(intptr_t)(g_engine.m_coordinator.GetSystem<GraphicsSystem>()->getFBO()), ImVec2(imageSize.x,imageSize.y ), ImVec2(0, 1), ImVec2(1, 0));
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("Level"))
			{
			//	std::string payload_n = *(std::string*)payload->Data;
			//	m_texturePath = payload_n.c_str();
			//	setTexture(m_texturePath.c_str());
			//	std::cout << payload_n.c_str() << std::endl;
			}
			ImGui::EndDragDropTarget();
		}

		ImVec2 mousePos = ImGui::GetMousePos();
		int width = g_engine.GetEngineWindowWidth();
		int height = g_engine.GetEngineWindowHeight();

		// Bring mousePos to the bottom left of the viewport
		mousePos.x -= ImGui::GetCursorScreenPos().x;
		mousePos.y -= ImGui::GetCursorScreenPos().y;

		// Bring mousePos to the top left of the viewport
		mousePos.y += imageSize.y;

		// Scale viewport coordinates to screen coordinates
		mousePos.x = mousePos.x * width / imageSize.x;
		mousePos.y = mousePos.y * height / imageSize.y;

		g_engine.SetViewportCursor(mousePos);
		ImGui::End();
	}
	void ImGuiEditorViewport::Shutdown()
	{

	}
	void ImGuiEditorViewport::ShowGizmo(Entity& selectedentity)
	{

		if (g_engine.m_coordinator.ComponentExists<TransformComponent>(selectedentity))
		{
			auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(selectedentity);
			float gizmoMatrix[16]{};
			float matrixScale[3]{}, matrixRotate[3]{}, matrixTranslate[3]{};
			ImVec2 m_Min = ImGui::GetWindowContentRegionMin();
			ImVec2 m_Max = ImGui::GetWindowContentRegionMax();
			ImVec2 imageSize = ImGui::GetContentRegionAvail();
			m_Min.x += ImGui::GetWindowPos().x;
			m_Min.y += ImGui::GetWindowPos().y;
			m_Max.x += ImGui::GetWindowPos().x;
			m_Max.y += ImGui::GetWindowPos().y;
			//std::cout << "height :" << x << "width: " << y << std::endl;
			matrixScale[0] = transform.GetScale().x;
			matrixScale[1] = transform.GetScale().y;
			matrixScale[2] = 1;

			float width = m_Max.x - m_Min.x;
			float height = m_Max.y - m_Min.y;
			
			matrixRotate[2] = transform.GetRotation();
			
			matrixTranslate[0] = transform.GetPosition().x;
			matrixTranslate[1] = transform.GetPosition().y;
			
			ImGuizmo::RecomposeMatrixFromComponents(matrixTranslate, matrixRotate, matrixScale, gizmoMatrix);
			ImGuiIO& io = ImGui::GetIO();
			
			ImGuizmo::SetRect(235, 75,imageSize.x, imageSize.y);
			ImGuizmo::Manipulate(glm::value_ptr(g_engine.m_coordinator.GetSystem<CameraSystem>()->GetViewMatrix(1.0f)),glm::value_ptr( g_engine.GetProjMat()), m_CurrentGizmoOperation, ImGuizmo::LOCAL, gizmoMatrix, NULL);
			ImGuizmo::DecomposeMatrixToComponents(gizmoMatrix, matrixTranslate, matrixRotate, matrixScale);


			switch (m_CurrentGizmoOperation)
			{
			case ImGuizmo::OPERATION::TRANSLATE:
				if (ImGuizmo::IsUsing())
				{
					Vec2 m_Translate{ matrixTranslate[0],matrixTranslate[1] };
					g_engine.m_coordinator.GetComponent<TransformComponent>(selectedentity).setPosition(m_Translate);
				}
				break;
			case ImGuizmo::OPERATION::ROTATE:
				if (ImGuizmo::IsUsing())
				{
					g_engine.m_coordinator.GetComponent<TransformComponent>(selectedentity).setRotation(matrixRotate[2]);
				}
				break;
			case ImGuizmo::OPERATION::SCALE:
				if (ImGuizmo::IsUsing())
				{
					Vec2 m_Scale{ matrixScale[0],matrixScale[1] };
					g_engine.m_coordinator.GetComponent<TransformComponent>(selectedentity).setScale(m_Scale);
				}
				break;
			default:
				break;
			}
			if (ImGuizmo::IsOver())
				ImGuizmo::Enable(true);
			else
				ImGuizmo::Enable(false);
		}


	}
}