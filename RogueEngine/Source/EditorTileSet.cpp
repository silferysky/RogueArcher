#include "Precompiled.h"
#include "EditorTileSet.h"
#include "REEngine.h"
#include "PickingManager.h"

namespace Rogue
{
	ImGuiTileSet::ImGuiTileSet() :m_TileSet(), m_GlobalTileSet()
	{
	}

	void ImGuiTileSet::Init()
	{
		if (m_TileSet.size())
		{
			m_TileSet.clear();
		}

		const AABB& viewportArea = PickingManager::instance().GetViewPortArea();
		m_minX = viewportArea.getMin().x;
		m_minY = viewportArea.getMin().y;
		m_maxX = viewportArea.getMax().x;
		m_maxY = viewportArea.getMax().y;
		TileSet tileset;
		while (m_minX < m_maxX)
		{
			tileset.m_tileTexture.m_texture = 0;
			tileset.m_texturename = "";
			tileset.m_tileTexture.m_data = 0;
			tileset.m_tileId = 0;
			m_currentTileX = round(m_minX / m_tileSize) * m_tileSize + m_tileSize / 2;
			tileset.m_tilePos.x = m_currentTileX;
			m_currentTileY = round(m_maxY / m_tileSize) * m_tileSize - m_tileSize / 2;
			tileset.m_tilePos.y = m_currentTileY;
			m_minX += m_tileSize;
			if (m_check)
			{
				++m_tilesWidth;
			}
			ImGuiTileSet::instance().m_TileSet.push_back(tileset);
			for (auto& i : ImGuiTileSet::instance().m_GlobalTileSet)
			{
				if (i.m_tilePos.x == m_currentTileX && i.m_tilePos.y == m_currentTileY)
				{
					//tileset.m_texturename = i.m_texturename;
					m_globalcheck = true;
					break;
				}
			}
			if (!m_globalcheck)
			{
				ImGuiTileSet::instance().m_GlobalTileSet.push_back(tileset);
			}
			if (m_minX > m_maxX && m_minY < m_maxY)
			{
				m_minX = viewportArea.getMin().x;
				m_maxY -= m_tileSize;
				++m_tilesHeight;
				m_check = false;
				m_globalcheck = false;
			}
		}

	}
	void ImGuiTileSet::Update()
	{
		if (!m_openWindow)
		{
			EditorManager::instance().RemoveEditorWindow<ImGuiTileSet>("TileSet");
			ImGuiInspector::instance().ReduceTileInstanceCount();
		}
		else
		{
			if (!ImGui::Begin("TileSet", &m_openWindow))
			{		
				ImGui::End();
			}
			else
			{
				ImGui::BeginChild("Tile");
				ImGui::Columns(2);
				ImGui::AlignTextToFramePadding();
				ImVec2 imageSize;
				imageSize.x = 20.0f;
				imageSize.y = 20.0f;
				int temp = ImGuiTileSet::instance().m_tilesWidth;
				for (auto& i : ImGuiTileSet::instance().m_TileSet)
				{	
					if (m_tilesWidth > 0)
					{
						ImGui::SameLine();
					}
					else
					{
						ImGui::NewLine();
						m_tilesWidth = temp;
					}
					ImGui::Image((void*)i.m_tileTexture.m_texture, ImVec2(imageSize.x, imageSize.y), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1,1, 1, 1), i.m_bordercolor);
					--m_tilesWidth;
					if (ImGui::IsItemClicked(0))
					{
						i.m_tileTexture = TextureManager::instance().loadTexture(m_currentPath.c_str());
						i.m_texturename = m_currentPath.c_str();
						if (m_currentmode == Mode::Drag)
						{
							if (!m_firstclicked)
							{
								m_firstclicked = true;
							}
							else
							{
								m_firstclicked = false;
							}
						}
						if (m_isCollision)
						{
							i.m_collision = true;
							i.m_bordercolor = { 0.8f,0.1f,0.1f,1.0f };
						}
					}
					if (m_currentmode == Mode::Drag)
					{
						if (ImGui::IsItemHovered() && m_firstclicked && m_currentPath != "")
						{
							i.m_tileTexture = TextureManager::instance().loadTexture(m_currentPath.c_str());
							i.m_texturename = m_currentPath.c_str();
							if(m_isCollision)
							{
								i.m_collision = true;
								i.m_bordercolor = { 0.8f,0.1f,0.1f,1.0f };
							}
						}
					}
				}
				ImGui::NextColumn();
				m_currentTexture = TextureManager::instance().loadTexture(m_currentPath.c_str());
				ImGui::Text("Current Image");
				ImGui::Image((void*)m_currentTexture.m_texture, ImVec2(150.0f, 150.f), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1, 1, 1, 1), ImGui::GetStyle().Colors[ImGuiCol_Border]);
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("Data"))
					{
						DirectoryInfo payload_n = *(DirectoryInfo*)payload->Data;
						if (payload_n.m_fileType == "png" || payload_n.m_fileType == "bmp")
						{
							m_currentPath = payload_n.m_filePath.c_str();
							ImGui::EndDragDropTarget();
						}
						else
						{
							ImGui::OpenPopup("Texture Error");
						}
					}
				}

				ImGui::Text("%s", m_currentPath.c_str());
				
				if (ImGui::RadioButton("Single Tile Mode", m_currentmode == Mode::Single))
				{
					m_currentmode = Mode::Single;
				}
				ImGui::SameLine();
				if (ImGui::RadioButton("Drag Tile Mode", m_currentmode == Mode::Drag))
				{
					m_currentmode = Mode::Drag;
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
					for (auto& i : ImGuiTileSet::instance().m_TileSet)
					{
						for (auto& j : ImGuiTileSet::instance().m_GlobalTileSet)
						{
							//if tile is used
							if (i.m_tileTexture.m_data != 0)
							{
								//find global tile
								if (i.m_tilePos.x == j.m_tilePos.x && i.m_tilePos.y == j.m_tilePos.y)
								{
									//if texture has been changed
									if (i.m_texturename != j.m_texturename)
									{
										//j.m_texturename = i.m_texturename;
										//j.m_tileTexture = i.m_tileTexture;
										j = i;
										//if tile exists, delete tile
										if (j.m_tileId > 0)
										{
											g_engine.m_coordinator.AddToDeleteQueue(j.m_tileId);
										}
										
										j.m_tileId = Create2DSprite(i.m_tilePos, Vec2{ 100.0f,100.0f }, i.m_texturename, i.m_collision);
									}
								}
							}
						
						}
					}
				}

				if (ImGui::Button("Delete current path"))
				{
					m_currentPath = "None";
				}

				if (ImGui::Checkbox("Collision On", &m_isCollision))
				{

				}
				ImGui::TextWrapped("When Drag Mode, first click to start drag and second click to stop drag");
				ImGui::TextWrapped("When Collision is on, the tile border will be red color");
				ImGui::EndChild();
				ImGui::End();
			}
		}
		
	}
	void ImGuiTileSet::Shutdown()
	{
		
	}



	Entity ImGuiTileSet::Create2DSprite(Vec2 position, Vec2 scale, std::string_view tilepath, bool iscollision)
	{
		Entity newEnt = g_engine.m_coordinator.CreateEntity();
		g_engine.m_coordinator.AddComponent<TransformComponent>(newEnt,TransformComponent(position, scale, 0.0f));
		if (iscollision)
		{
			auto& rigid = g_engine.m_coordinator.CreateComponent<RigidbodyComponent>(newEnt);
			rigid.setIsStatic(true);
			g_engine.m_coordinator.AddComponent<BoxCollider2DComponent>(newEnt, BoxCollider2DComponent());
			g_engine.m_coordinator.AddComponent<ColliderComponent>(newEnt, ColliderComponent(std::make_shared<BoxShape>()));
		}
		auto& Sprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(newEnt);
		Sprite.setTexturePath(tilepath);
		return newEnt;
	}
}