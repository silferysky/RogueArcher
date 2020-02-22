#include "Precompiled.h"
#include "EditorTileSet.h"
#include "REEngine.h"
#include "PickingManager.h"

namespace Rogue
{
	std::string ImGuiTileSet::Serialize()
	{
		std::ostringstream oss;

		for (auto& tile : m_GlobalTileSet)
		{
			oss << tile.Serialize() << "|";
		}

		return oss.str();
	}

	void ImGuiTileSet::Deserialize(std::string_view deserializeStr)
	{
		std::istringstream iss(deserializeStr.data());
		std::string str;

		while (std::getline(iss, str, '|'))
		{
			Tile tile;
			tile.Deserialize(str);
			m_GlobalTileSet.push_back(tile);
		}
	}

	ImGuiTileSet::ImGuiTileSet() :m_TileSet(), m_GlobalTileSet(),
		m_minY(0),m_minX(0),m_maxX(0),m_maxY(0),m_tileSize(100),m_tilesHeight(0),
		m_tilesWidth(0),m_currentTileX(0),m_currentTileY(0),m_openWindow(true), m_isCollision(false),
		m_check(true), m_firstclicked(false), m_globalcheck(false), m_deleteTile(false)
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
		Tile tileset;
		m_tilesWidth = 0;
		m_tilesHeight = 0;
		m_check = true;

		while (m_minX < m_maxX)
		{
			tileset.m_tileTexture.m_texture = 0;
			tileset.m_texturename = "";
			tileset.m_tileTexture.m_data = 0;
			tileset.m_tileId = 0;
			m_currentTileX = round(m_minX / m_tileSize) * m_tileSize + m_tileSize;
			tileset.m_tilePos.x = m_currentTileX;
			m_currentTileY = round(m_maxY / m_tileSize) * m_tileSize - m_tileSize;
			tileset.m_tilePos.y = m_currentTileY;
			m_minX += m_tileSize;
			if (m_check)
			{
				++m_tilesWidth;
			}
			for (auto& i : ImGuiTileSet::instance().m_GlobalTileSet)
			{
				if (i.m_tilePos.x == m_currentTileX && i.m_tilePos.y == m_currentTileY)
				{
					tileset.m_tileTexture = i.m_tileTexture;
					tileset.m_bordercolor = i.m_bordercolor;
					tileset.m_texturename = i.m_texturename;
					m_globalcheck = true;
					break;
				}
			}
			
			ImGuiTileSet::instance().m_TileSet.push_back(tileset);
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
				ImGui::BeginChild("##1", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
				ImGui::SetNextWindowContentWidth(1500);
				ImGui::AlignTextToFramePadding();

				ImVec2 imageSize;
				imageSize.x = 20.0f;
				imageSize.y = 20.0f;
				int temp = ImGuiTileSet::instance().m_tilesWidth;
				int width = ImGuiTileSet::instance().m_tilesWidth;
				for (auto& i : ImGuiTileSet::instance().m_TileSet)
				{	
					if (width > 0)
					{
						ImGui::SameLine();
					}
					else
					{
						ImGui::NewLine();
						width = temp;
					}
					--width;
					ImGui::Image((void*)i.m_tileTexture.m_texture, ImVec2(imageSize.x, imageSize.y), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1,1, 1, 1), i.m_bordercolor);

					if (ImGui::IsItemClicked(0))
					{
						if (m_deleteTile)
						{
							i.m_tileTexture = TextureManager::instance().loadTexture("None");
							i.m_texturename = "None";
							i.m_bordercolor = { 1.0f, 1.0f, 1.0f, 0.5f };
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
						}
						else
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
					}
					if (m_currentmode == Mode::Drag)
					{
						if (!m_deleteTile)
						{
							if (ImGui::IsItemHovered() && m_firstclicked && m_currentPath != "None")
							{
								i.m_tileTexture = TextureManager::instance().loadTexture(m_currentPath.c_str());
								i.m_texturename = m_currentPath.c_str();
								if (m_isCollision)
								{
									i.m_collision = true;
									i.m_bordercolor = { 0.8f,0.1f,0.1f,1.0f };
								}
							}
						}
						else
						{
							if (ImGui::IsItemHovered() && m_firstclicked)
							{
								i.m_tileTexture = TextureManager::instance().loadTexture("None");
								i.m_texturename = "None";
								i.m_bordercolor = { 1.0f, 1.0f, 1.0f, 0.5f };
							}					
						}
					}
				}
				ImGui::EndChild();
				ImGui::NextColumn();
				m_currentTexture = TextureManager::instance().loadTexture(m_currentPath.c_str());
				ImGui::Text("Current Image");
				ImGui::Image((void*)m_currentTexture.m_texture, ImVec2(50.0f, 50.f), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1, 1, 1, 1), ImGui::GetStyle().Colors[ImGuiCol_Border]);
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("Data"))
					{
						DirectoryInfo payload_n = *(DirectoryInfo*)payload->Data;
						if (payload_n.m_fileType == "png" || payload_n.m_fileType == "bmp")
						{
							m_currentPath = payload_n.m_filePath.c_str();
							m_tilePallete.clear();
							for (int i = 0; i < 5; ++i)
							{
								float m_tempmin = 0;
								float m_tempmax = 0.2f;
								for (int j = 0; j < 5; ++j)
								{
									TilePallet pallet;
									pallet.m_texture = TextureManager::instance().loadTexture(m_currentPath.c_str());
									pallet.m_texCoordMin = m_tempmin;
									pallet.m_texCoordMax = m_tempmax;
									m_tempmin += 0.2f;
									m_tempmax += 0.2f;
								}
							}
							ImGui::EndDragDropTarget();
						}
						else
						{
							ImGui::OpenPopup("Texture Error");
						}
					}
				}
				for (int i = 0; i < 5; ++i)
				{
					ImGui::NewLine();				
					for (int j = 0; j < 5; ++j)
					{
						ImGui::SameLine();
						ImGui::Image((void*)m_currentTexture.m_texture, ImVec2(50.0f, 50.f), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1, 1, 1, 1), ImGui::GetStyle().Colors[ImGuiCol_Border]);
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
							//find global tile
							if (i.m_tilePos.x == j.m_tilePos.x && i.m_tilePos.y == j.m_tilePos.y)
							{
								
								//if texture has been changed
								if (i.m_texturename != j.m_texturename)
								{
									j.m_texturename = i.m_texturename;
									j.m_tileTexture = i.m_tileTexture;
									j.m_bordercolor = i.m_bordercolor;
									//std::cout << j.m_texturename << std::endl;
									//if tile exists, delete tile
									if (j.m_tileId > 0)
									{
										g_engine.m_coordinator.AddToDeleteQueue(j.m_tileId);
									}
									if (j.m_texturename.c_str() == "None")
									{
										g_engine.m_coordinator.AddToDeleteQueue(j.m_tileId);
									}
									
									j.m_tileId = Create2DSprite(i.m_tilePos, Vec2{ 100,100 }, i.m_texturename, i.m_collision);
								}
							}
						}
					}
					for (auto& i : ImGuiTileSet::instance().m_GlobalTileSet)
					{
						if (g_engine.m_coordinator.ComponentExists<SpriteComponent>(i.m_tileId))
						{
							auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(i.m_tileId);
								sprite.setTexCoordMin(0.0f);
								sprite.setTexCoordMax(0.25f);
						}
					}
					std::string file = SceneManager::instance().getCurrentFileName().c_str();
					SceneManager::instance().SaveTileset(file.c_str());
				}

				if (ImGui::Button("Delete current path"))
				{
					m_currentPath = "None";
				}

				if (ImGui::Checkbox("Collision On", &m_isCollision))
				{

				}

				if (ImGui::Checkbox("Delete Tile", &m_deleteTile))
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
		m_TileSet.clear();

	}

	Entity ImGuiTileSet::Create2DSprite(Vec2 position, Vec2 scale, std::string_view tilepath, bool iscollision)
	{
		Entity newEnt = g_engine.m_coordinator.CreateEntity();
		if (iscollision)
		{
			auto& rigid = g_engine.m_coordinator.CreateComponent<RigidbodyComponent>(newEnt);
			rigid.setIsStatic(true);
			g_engine.m_coordinator.AddComponent<BoxCollider2DComponent>(newEnt, BoxCollider2DComponent());
			g_engine.m_coordinator.AddComponent<ColliderComponent>(newEnt, ColliderComponent(std::make_shared<BoxShape>()));
		}
		auto& trans = g_engine.m_coordinator.CreateComponent<TransformComponent>(newEnt);
		trans.setPosition(position);
		trans.setScale(scale);
		trans.setZ(100);
		auto& Sprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(newEnt);
		Sprite.setTexturePath(tilepath); 
		return newEnt;
	}

	std::string Tile::Serialize()
	{
		std::ostringstream oss;
		//oss << m_tileId << ";";
		oss << m_texturename << ";";
		oss << m_tilePos.x << "," << m_tilePos.y << ";";
		oss << m_collision << ";";
		oss << m_bordercolor.w << "," << m_bordercolor.x << "," << m_bordercolor.y << "," << m_bordercolor.z << ";";

		return oss.str();
	}

	void Tile::Deserialize(std::string_view deserializeStr)
	{
		std::istringstream iss(deserializeStr.data());
		std::string str;

		if (std::getline(iss, str, ';'))
		{
			m_texturename = str;
			m_tileTexture = TextureManager::instance().loadTexture(str.c_str());
		}

		if (std::getline(iss, str, ','))
		{
			m_tilePos.x = std::stof(str);
		}
		if (std::getline(iss, str, ';'))
		{
			m_tilePos.y = std::stof(str);
		}

		if (std::getline(iss, str, ';'))
		{
			m_collision = std::stoi(str);
		}

		if (std::getline(iss, str, ','))
		{
			m_bordercolor.w = std::stof(str);
		}
		if (std::getline(iss, str, ','))
		{
			m_bordercolor.x = std::stof(str);
		}
		if (std::getline(iss, str, ','))
		{
			m_bordercolor.y = std::stof(str);
		}
		if (std::getline(iss, str, ';'))
		{
			m_bordercolor.z = std::stof(str);
		}

		//Creating a sprite
		if (m_texturename != "None" && m_texturename != "")
		{
			//std::cout << "Create: " << m_texturename << std::endl;
			ImGuiTileSet::instance().Create2DSprite(m_tilePos, Vec2(100.0f, 100.0f), m_texturename, m_collision);
		}

		//Entity tileEntity = g_engine.m_coordinator.CreateEntity();

		//g_engine.m_coordinator.AddComponent<SpriteComponent>(tileEntity, SpriteComponent());
		//g_engine.m_coordinator.AddComponent<TransformComponent>(tileEntity, TransformComponent());
		//g_engine.m_coordinator.TryGetComponent<SpriteComponent>(tileEntity)->get().setTexturePath(m_texturename);
		//g_engine.m_coordinator.TryGetComponent<TransformComponent>(tileEntity)->get().setPosition(m_tilePos);


		//if (m_collision)
		//{
		//	g_engine.m_coordinator.AddComponent<RigidbodyComponent>(tileEntity, RigidbodyComponent());
		//	g_engine.m_coordinator.AddComponent<BoxCollider2DComponent>(tileEntity, BoxCollider2DComponent());
		//}
	}
}