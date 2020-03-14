/* Start Header ************************************************************************/
/*!
\file           EditorViewport.cpp
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for EditorViewport

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "EditorViewport.h"
#include "PickingManager.h"
#include "CameraManager.h"
#include "REMath.h"
#include "CollisionManager.h"

namespace Rogue
{
	ImGuiEditorViewport::ImGuiEditorViewport() :
		m_currentVector{ g_engine.m_coordinator.GetActiveObjects() }, m_CurrentGizmoOperation{ ImGuizmo::TRANSLATE }, m_CurrentGizmoMode{ }
	{
	}
	void ImGuiEditorViewport::Init()
	{}

	void ImGuiEditorViewport::Update()
	{
		ImGui::Begin("Viewport");
		ImGui::BeginChild("Viewport child");
		ImGuiStyle& style = ImGui::GetStyle();
		if (ImGui::Button("Play"))
		{
			//If game is not running, save level, and set it to running
			if (!g_engine.m_coordinator.GetGameState())
			{
				g_engine.SetTimeScale(1.0f);
				SceneManager::instance().SaveLevel(SceneManager::instance().getCurrentFileName().c_str());
				CameraManager::instance().SetCameraZoom(CameraManager::instance().GetLevelCameraZoom());
				CameraManager::instance().SetCameraPos(glm::vec3(PLAYER_STATUS.GetStartingPos().x, PLAYER_STATUS.GetStartingPos().y, CameraManager::instance().GetCameraPos().z));
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
		ImGui::PushItemWidth(75);
		
		if (ImGui::Button("Play Without Saving"))
		{
			if (!g_engine.m_coordinator.GetGameState())
			{
				CameraManager::instance().SetCameraZoom(CameraManager::instance().GetLevelCameraZoom());
				CameraManager::instance().SetCameraPos(glm::vec3(PLAYER_STATUS.GetStartingPos().x, PLAYER_STATUS.GetStartingPos().y, CameraManager::instance().GetCameraPos().z));
				g_engine.m_coordinator.SetGameState(true);
				//ShowCursor(false);
			}
		}

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Instantly plays the scene without saving");
			ImGui::EndTooltip();
		}
		if (!g_engine.m_coordinator.GetGameState() || g_engine.m_coordinator.GetPauseState())
		{
			if (g_engine.GetIsFocused())
			{
				if (ImGui::IsKeyPressed('Q') && g_engine.GetIsFocused())
				{
					m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
				}
				if (ImGui::IsKeyPressed('W') && g_engine.GetIsFocused())
				{
					m_CurrentGizmoOperation = ImGuizmo::ROTATE;
				}
				if (ImGui::IsKeyPressed('E') && g_engine.GetIsFocused())
				{
					m_CurrentGizmoOperation = ImGuizmo::SCALE;
				}
			}
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
		if (!g_engine.m_coordinator.GetGameState() || g_engine.m_coordinator.GetPauseState())
		{
			for (auto& i : m_currentVector)
			{
				HierarchyInfo& objInfo = g_engine.m_coordinator.GetHierarchyInfo(i);
				if (objInfo.m_selected == true)
				{
					const AABB& viewportArea = PickingManager::instance().GetViewPortArea();
					TransformComponent& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(i);
					Vec2 pos = trans.GetPosition();
					if (CollisionManager::instance().DiscretePointVsAABB(pos, viewportArea) /*&& ImGui::IsWindowFocused()*/)
					{
						ShowGizmo(i);
					}
				}
					
			}
		}

		ImVec2 imageSize = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)(intptr_t)(g_engine.m_coordinator.GetSystem<GraphicsSystem>()->getFBO()), ImVec2(imageSize.x,imageSize.y ), ImVec2(0, 1), ImVec2(1, 0));
		auto drawlist = ImGui::GetWindowDrawList();
		ImVec2 TileSize(64.0f, 64.0f);
		ImGui::GetWindowContentRegionMin();
		//drawlist->AddLine(ImVec2(1000.0f, 1000.0f), ImVec2(100.0f, 100.0f), ImColor(120, 100, 100), 3.0f);
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("Data"))
			{
				DirectoryInfo payload_n = *(DirectoryInfo*)payload->Data;
				size_t levelcheck = payload_n.m_filePath.find_last_of("Level");
				std::cout << levelcheck << std::endl;
				size_t filetype = payload_n.m_filePath.find_last_of(".");
				std::string levelnumber = payload_n.m_filePath.substr(levelcheck - 4, filetype);
				std::string level = levelnumber.substr(0, 5);
				if (payload_n.m_fileType == "json" && level == ("Level"))
				{
						SceneManager& sceneManager = SceneManager::instance();
						std::cout << "load!" << std::endl;
						sceneManager.LoadLevel(levelnumber);
						
						g_engine.m_coordinator.SetGameState(false);
						g_engine.m_coordinator.SetPauseState(false);
						
					ImGui::EndDragDropTarget();
				}
				else if (payload_n.m_fileType == "json")
				{
					size_t filetype = payload_n.m_fileName.find_last_of(".");
					std::string fileNameWithoutType = payload_n.m_fileName.substr(0, filetype);
					for (auto& i : SceneManager::instance().GetArchetypeMap())
					{
						SceneManager::instance().Clone(fileNameWithoutType.c_str());
						std::cout << "clone!" << std::endl;
					}
				}

				else
				{
					ImGui::OpenPopup("File Error");
				}
			}
		}

		bool open = true;
		if (ImGui::BeginPopupModal("File Error", &open))
		{
			ImGui::Text("Error!, Please only put in level files or prefabs!");
			if (ImGui::Button("Close"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
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

		PickingManager::instance().SetViewPortCursor(mousePos);
		ImGui::EndChild();
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
			float buttonOffset = 21.0f;
			ImVec2 m_Min = ImGui::GetWindowContentRegionMin();
			ImVec2 m_Max = ImGui::GetWindowContentRegionMax();
			ImVec2 imageSize = ImGui::GetContentRegionAvail();
			ImVec2 mousePos = ImGui::GetMousePos();
			
			m_Min.x += ImGui::GetWindowPos().x;
			m_Min.y += ImGui::GetWindowPos().y;
			m_Max.x += ImGui::GetWindowPos().x;
			m_Max.y += ImGui::GetWindowPos().y;
			const AABB& viewportArea = PickingManager::instance().GetViewPortArea();
			
			matrixScale[0] = abs(transform.GetScale().x);
			matrixScale[1] = abs(transform.GetScale().y);
			matrixScale[2] = 1;

			float width = m_Max.x - m_Min.x;
			float height = m_Max.y - m_Min.y;
			
			matrixRotate[2] = transform.GetRotation();
			
			matrixTranslate[0] = transform.GetPosition().x;
			matrixTranslate[1] = transform.GetPosition().y;

			ImGuizmo::RecomposeMatrixFromComponents(matrixTranslate, matrixRotate, matrixScale, gizmoMatrix);
			ImGuiIO& io = ImGui::GetIO();

			ImVec2 size = ImGui::GetWindowSize();
			ImGuizmo::SetRect(m_Min.x, m_Min.y + buttonOffset, imageSize.x, imageSize.y);
			//ImGui::GetWindowDrawList()->PushClipRect(m_Min, m_Max, false);
			//ImGui::GetWindowDrawList()->PushClipRect(m_Min, m_Max, false);
			//ImGuizmo::SetDrawlist();
			
			//if (mousePos.x < m_Min.x)
			//{
			//	return;
			//}
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