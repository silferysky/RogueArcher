#include "Precompiled.h"
#include "EditorTileSet.h"
#include "REEngine.h"
#include "PickingManager.h"

namespace Rogue
{
	void ImGuiTileSet::Init()
	{
		if (m_TileSet.size())
		{
			m_TileSet.clear();
		}
		const AABB& viewportArea = PickingManager::instance().GetViewPortArea();
		m_viewportWidth = viewportArea.getMax().x - viewportArea.getMin().x;
		m_viewportHeight = viewportArea.getMax().y - viewportArea.getMin().y;
		
		int m_id = 0;
		TileSet tileset;
		while (m_viewportHeight > 0)
		{
			tileset.m_tileId = m_id;
			tileset.m_texturename = "";
			m_TileSet.push_back(tileset);
			++m_id;
			while (m_viewportWidth > 0)
			{
				tileset.m_tileId = m_id;
				m_viewportWidth -= m_tileSize;
				m_TileSet.push_back(tileset);
				++m_id;
				++m_tilesWidth;
				break;
			}
			m_viewportHeight -= m_tileSize;
			++m_tilesHeight;
		}
	}
	void ImGuiTileSet::Update()
	{
		if (m_openWindow)
		{
			if (!ImGui::Begin("TileSet", &m_openWindow))
			{
				ImGui::End();
			}
			else
			{
				ImGui::BeginChild("Test");
				auto drawlist = ImGui::GetWindowDrawList();

				//ImVec2 test;
				//drawlist->AddLine(test, ImVec2(100.0f, 100.9f), ImColor(120, 100, 100), 3.0f);
				ImGui::Columns(2);
				ImVec2 imageSize;
				imageSize.x = 50.0f;
				imageSize.y = 50.0f;
				for (int j = 0; j < 10;++j)
				{
					ImGui::NewLine();
					for (int i = 0; i < 10; ++i)
					{
						ImGui::SameLine();
						ImGui::Image((void*)(intptr_t)(g_engine.m_coordinator.GetSystem<GraphicsSystem>()->getFBO()), ImVec2(imageSize.x, imageSize.y), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1, 1, 1, 1), ImGui::GetStyle().Colors[ImGuiCol_Border]);
						if (ImGui::IsItemClicked(0))
						{
							
						}
					}
				}

				
				ImGui::NextColumn();
				ImGui::Text("Current Image");
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("Data"))
					{
						DirectoryInfo payload_n = *(DirectoryInfo*)payload->Data;
						if (payload_n.m_fileType == "png" || payload_n.m_fileType == "bmp")
						{
							//m_texturePath = payload_n.m_filePath.c_str();
							//setTexture(m_texturePath.c_str());
							ImGui::EndDragDropTarget();
						}

						else
						{
							ImGui::OpenPopup("Texture Error");
						}
					}
				}
				bool open = true;
				if (ImGui::BeginPopupModal("Texture Error", &open))
				{
					ImGui::Text("Error!, Please only put in texture files!");
					if (ImGui::Button("Close"))
						ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
				}

				if (ImGui::Button("Save current TileSet"))
				{

				}

				if (ImGui::Checkbox("Collision On", &m_isCollision))
				{

				}
				ImGui::EndChild();
				ImGui::End();
			}
		}


	}
	void ImGuiTileSet::Shutdown()
	{
	}

	Entity ImGuiTileSet::Create2DSprite()
	{
		Entity newEnt = g_engine.m_coordinator.CreateEntity();
		g_engine.m_coordinator.AddComponent<TransformComponent>(
			newEnt,
			TransformComponent(Vec2{ 0.0f, 0.0f }, Vec2{ 100.0f, 100.0f }, 0.0f));

		auto& Sprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(newEnt);
		Sprite.Deserialize("Resources/Assets/DefaultSprite.png;1;1;1;1;1");
		return newEnt;
	}
}