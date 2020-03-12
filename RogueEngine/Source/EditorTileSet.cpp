#include "Precompiled.h"
#include "EditorTileSet.h"
#include "REEngine.h"
#include "PickingManager.h"
#include "Logger.h"
#include "EditorManager.h"

#define OLD_SERIALIZATION 0

namespace Rogue
{
	std::string ImGuiTileSet::Serialize()
	{
		auto tilemapCompOpt = g_engine.m_coordinator.TryGetComponent<TileMapComponent>(m_tileMapEnt);
		auto spriteOpt = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(m_tileMapEnt);
		
		if (!tilemapCompOpt || !spriteOpt)
			return std::string();

		TileMapComponent& tilemapComp = tilemapCompOpt->get();
		SpriteComponent& sprite = spriteOpt->get();

		std::ostringstream oss;

		oss << sprite.getTexturePath() << "|";

		for (TrueTile& trueTile : tilemapComp.GetTileMap())
		{
			oss << trueTile.Serialize() << "|";
		}

		//for (auto& tile : m_GlobalTileSet)
		//{
		//	if (tile.m_texturename != "" && tile.m_texturename != "Resources\\Assets\\tile.png")
		//	{
		//		tiles++;
		//		oss << tile.Serialize() << "|";
		//	}

		//	std::stringstream texture;
		//	texture << "Texture name: " << tile.m_texturename;

		//	RE_INFO(texture.str());
		//}

		std::stringstream ss;
		ss << "Size of vector: " << tilemapComp.GetTileMap().size();
		RE_INFO(ss.str());

		return oss.str();
	}

	void ImGuiTileSet::Deserialize(std::string_view deserializeStr)
	{
		std::istringstream iss(deserializeStr.data());
		std::string str;

		// Deserialize the texture first
		if (std::getline(iss, str, '|'))
			m_currentPath = str;

		m_currentTexture = TextureManager::instance().loadTexture(m_currentPath.c_str());

		m_tileMapEnt = Create2DSprite(Vec2(0.0f, 0.0f), Vec2(61, 61), m_currentPath);
		TileMapComponent& tilemapComp = g_engine.m_coordinator.CreateComponent<TileMapComponent>(m_tileMapEnt);
		TileMap& tilemap = tilemapComp.GetTileMap();

		// Deserialize each tile and push into tilemap
		while (std::getline(iss, str, '|'))
		{
			TrueTile trueTile;
			trueTile.Deserialize(str);

			tilemap.emplace_back(trueTile);
		}
	}

	ImGuiTileSet::ImGuiTileSet() :m_TileSet(),
		m_minY(0), m_minX(0), m_maxX(0), m_maxY(0), m_tileSize(61), m_tilesHeight(0),
		m_tilesWidth(0), m_currentTileX(0), m_currentTileY(0), m_openWindow(true), m_isCollision(false),
		m_check(true), m_firstclicked(false), m_globalcheck(false), m_deleteTile(false), m_hasTextureChanged(false), m_groundtag(false),
		m_tileMapEnt(MAX_ENTITIES)
	{
	}

	void ImGuiTileSet::Init()
	{
		m_TileSet.clear();

		const AABB& viewportArea = PickingManager::instance().GetViewPortArea();
		m_minX = viewportArea.getMin().x;
		m_minY = viewportArea.getMin().y;
		m_maxX = viewportArea.getMax().x;
		m_maxY = viewportArea.getMax().y;
		Tile tileset;
		m_tilesWidth = 0;
		m_tilesHeight = 0;
		m_check = true;
		m_openWindow = true;

		TileMapComponent* pTilemap = nullptr;

		if (m_tileMapEnt != MAX_ENTITIES)
			pTilemap = &g_engine.m_coordinator.GetComponent<TileMapComponent>(m_tileMapEnt);
		else
			pTilemap = &g_engine.m_coordinator.CreateComponent<TileMapComponent>(m_tileMapEnt);

		while (m_minX < m_maxX)
		{
			tileset.m_tileTexture.m_texture = 0;
			tileset.m_texturename = "Resources\\Assets\\tile.png";
			tileset.m_tileTexture = TextureManager::instance().loadTexture("Resources\\Assets\\tile.png");
			tileset.m_tileTexture.m_data = 0;
			//tileset.m_tileId = 0;
			m_currentTileX = round(m_minX / m_tileSize) * m_tileSize;
			tileset.m_tilePos.x = m_currentTileX;
			m_currentTileY = round(m_maxY / m_tileSize) * m_tileSize;
			tileset.m_tilePos.y = m_currentTileY;
			m_minX += m_tileSize;
			if (m_check)
			{
				++m_tilesWidth;
			}

			if (pTilemap)
			{
				for (TrueTile trueTile : pTilemap->GetTileMap())
				{
					if (trueTile.m_tilePos.x == m_currentTileX && trueTile.m_tilePos.y == m_currentTileY)
					{
						tileset.m_texCoordMinX = trueTile.m_min.x;
						tileset.m_texCoordMaxX = trueTile.m_max.x;
						tileset.m_texCoordMinY = trueTile.m_min.y;
						tileset.m_texCoordMaxY = trueTile.m_max.y;

						tileset.m_texturename = m_currentPath;
						tileset.m_tileTexture = m_currentTexture;

						m_globalcheck = true;
						break;
					}
				}
			}
			
			m_TileSet.emplace_back(tileset);

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
			//EditorManager::instance().RemoveEditorWindow<ImGuiTileSet>("TileSet");
			EditorManager::instance().ReduceTileInstanceCount();
			EditorManager::instance().setTileSetOpen(false);
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
				int temp = m_tilesWidth;
				int width = m_tilesWidth;
				for (auto& i : m_TileSet)
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
					ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<unsigned long long>(i.m_tileTexture.m_texture)),
						ImVec2(imageSize.x, imageSize.y),
						ImVec2(i.m_texCoordMinX, i.m_texCoordMaxY),
						ImVec2(i.m_texCoordMaxX, i.m_texCoordMinY),
						ImVec4(1,1, 1, 1),
						ImVec4(1.0f, 1.0f, 1.0f, 0.5f));

					if (ImGui::IsItemClicked(0))
					{
						if (m_deleteTile)
						{
							i.m_tileTexture = TextureManager::instance().loadTexture("Resources\\Assets\\tile.png");
							i.m_texturename = "Resources\\Assets\\tile.png";

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
							m_currentTexture = TextureManager::instance().loadTexture(m_currentPath.c_str());
							i.m_texCoordMinX = m_minX;
							i.m_texCoordMaxX = m_maxX;
							i.m_texCoordMinY = m_minY;
							i.m_texCoordMaxY = m_maxY;

							i.m_texturename = m_currentPath;
							i.m_tileTexture = m_currentTexture;
							
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
					}
					if (m_currentmode == Mode::Drag)
					{
						if (!m_deleteTile)
						{
							if (ImGui::IsItemHovered() && m_firstclicked && m_currentPath != "Resources\\Assets\\tile.png")
							{
								i.m_texturename = m_currentPath;
								i.m_tileTexture = m_currentTexture;

								i.m_texCoordMinX = m_minX;
								i.m_texCoordMaxX = m_maxX;
								i.m_texCoordMinY = m_minY;
								i.m_texCoordMaxY = m_maxY;
							}
						}
						else
						{
							if (ImGui::IsItemHovered() && m_firstclicked)
							{
								i.m_tileTexture = TextureManager::instance().loadTexture("Resources\\Assets\\tile.png");
								i.m_texturename = "Resources\\Assets\\tile.png";
							}					
						}
					}
				}
				ImGui::EndChild();
				ImGui::NextColumn();
				m_currentTexture = TextureManager::instance().loadTexture(m_currentPath.c_str());

				ImGui::Text("Current Selected Tile");
				ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<unsigned long long>(m_currentTexture.m_texture)),
					ImVec2(50.0f, 50.f),
					ImVec2(m_minX, m_maxY),
					ImVec2(m_maxX, m_minY),
					ImVec4(1, 1, 1, 1),
					ImGui::GetStyle().Colors[ImGuiCol_Border]);

				ImGui::Text("Current Image");
				ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<unsigned long long>(m_currentTexture.m_texture)),
					ImVec2(50.0f, 50.f),
					ImVec2(0, 1), ImVec2(1, 0),
					ImVec4(1, 1, 1, 1),
					ImGui::GetStyle().Colors[ImGuiCol_Border]);

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload * payload = ImGui::AcceptDragDropPayload("Data"))
					{
						DirectoryInfo payload_n = *(DirectoryInfo*)payload->Data;
						if (payload_n.m_fileType == "png" || payload_n.m_fileType == "bmp")
						{
							m_currentPath = payload_n.m_filePath.c_str();
							m_hasTextureChanged = true;
							ImGui::EndDragDropTarget();
						}
						else
						{
							ImGui::OpenPopup("Texture Error");
						}
					}
				}
				float minx = 0.0f;
				float maxx = 0.2f;
				float miny = 0.8f;
				float maxy = 1.0f;

				for (int i = 0; i < 5; ++i)
				{
					ImGui::NewLine();				
					for (int j = 0; j < 5; ++j)
					{
						ImGui::SameLine();
						ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<unsigned long long>(m_currentTexture.m_texture)),
							ImVec2(50.0f, 50.0f),
							ImVec2(minx, maxy),
							ImVec2(maxx, miny),
							ImVec4(1, 1, 1, 1),
							ImGui::GetStyle().Colors[ImGuiCol_Border]);
						
						if (ImGui::IsItemClicked(0))
						{
							m_minX = minx;
							m_minY = miny;
							m_maxX = maxx;
							m_maxY = maxy;
						}
						minx += 0.2f;
						maxx += 0.2f;
					}
					minx = 0.0f;
					maxx = 0.2f;
					miny -= 0.2f;
					maxy -= 0.2f;
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
				//	for (auto& i : m_TileSet)
				//	{
				//		for (auto& j : m_GlobalTileSet)
				//		{
				//			//find global tile
				//			if (i.m_tilePos.x == j.m_tilePos.x && i.m_tilePos.y == j.m_tilePos.y)
				//			{
				//				//m_globalcheck = true;
				//				//if texture has been changed, just change the texture
				//				//if ((i.m_texturename != j.m_texturename) /*&& i.m_texturename == "Resources\\Assets\\tile.png"*/)
				//				//{
				//				//	j.m_texCoordMinX = i.m_texCoordMinX;
				//				//	j.m_texCoordMaxX = i.m_texCoordMaxX;
				//				//	j.m_texCoordMinY = i.m_texCoordMinY;
				//				//	j.m_texCoordMaxY = i.m_texCoordMaxY;
				//				//	g_engine.m_coordinator.AddToDeleteQueue(j.m_tileId);
				//				//	
				//				//	if (i.m_texturename == "Resources\\Assets\\tile.png")
				//				//		continue;
				//				//	j.m_tileId = Create2DSprite(i.m_tilePos, Vec2{m_tileSize,m_tileSize }, i.m_texturename);
				//				//	auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(j.m_tileId);
				//				//	sprite.setTexCoordMinX(j.m_texCoordMinX);
				//				//	sprite.setTexCoordMaxX(j.m_texCoordMaxX);
				//				//	sprite.setTexCoordMinY(j.m_texCoordMinY);
				//				//	sprite.setTexCoordMaxY(j.m_texCoordMaxY);
				//				//	continue;
				//				//}
				//				//if (i.m_texturename != j.m_texturename)
				//				//{
				//				//	j.m_texturename = i.m_texturename;
				//				//	j.m_tileTexture = i.m_tileTexture;
				//				//	j.m_bordercolor = i.m_bordercolor;
				//				//	j.m_collision = i.m_collision;
				//				//	j.m_texCoordMinX = i.m_texCoordMinX;
				//				//	j.m_texCoordMaxX = i.m_texCoordMaxX;
				//				//	j.m_texCoordMinY = i.m_texCoordMinY;
				//				//	j.m_texCoordMaxY = i.m_texCoordMaxY;
				//				//
				//				//	g_engine.m_coordinator.AddToDeleteQueue(j.m_tileId);
				//				//	
				//				//
				//				//}
				//			}
				//		}
				//		//if current tile does not exist in global tile set
				//		//if (!m_globalcheck)
				//		//{
				//		//	if (i.m_texturename == "" || i.m_texturename == "Resources\\Assets\\tile.png" || m_hasTextureChanged)
				//		//	{
				//		//		m_globalcheck = false;
				//		//		m_hasTextureChanged = false;
				//		//		return;
				//		//	}
				//		//	else
				//		//	{
				//		//		i.m_tileId = Create2DSprite(i.m_tilePos, Vec2{m_tileSize,m_tileSize }, i.m_texturename);
				//		//		auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(i.m_tileId);
				//		//		sprite.setTexCoordMinX(i.m_texCoordMinX);
				//		//		sprite.setTexCoordMaxX(i.m_texCoordMaxX);
				//		//		sprite.setTexCoordMinY(i.m_texCoordMinY);
				//		//		sprite.setTexCoordMaxY(i.m_texCoordMaxY);
				//		//		m_GlobalTileSet.push_back(i);
				//		//	}
				//		//}
				//		m_globalcheck = false;
				//		m_hasTextureChanged = false;
				//	}
					if (m_tileMapEnt != MAX_ENTITIES)
					{
						auto& tilemap = g_engine.m_coordinator.GetComponent<TileMapComponent>(m_tileMapEnt);

						// Can only support 1 tilemap for now
						SaveTileMap(tilemap.GetTileMap());
					}
					else
					{
						m_tileMapEnt = Create2DSprite(Vec2(0.0f, 0.0f), Vec2(61, 61), m_currentPath);
						auto& tilemap = g_engine.m_coordinator.CreateComponent<TileMapComponent>(m_tileMapEnt);

						SaveTileMap(tilemap.GetTileMap());
					}
					std::string file = SceneManager::instance().getCurrentFileName().c_str();
					SceneManager::instance().SaveTileset(file.c_str());
				}

				if (ImGui::Button("Delete current path"))
				{
					m_currentPath = "Resources\\Assets\\tile.png";
				}

				if (ImGui::Checkbox("Collision On", &m_isCollision))
				{
					
				}

				if (ImGui::Checkbox("Delete Tile", &m_deleteTile))
				{

				}
				ImGui::TextWrapped("When Drag Mode, first click to start drag and second click to stop drag");
				ImGui::EndChild();
				ImGui::End();
			}
		}
		
	}
	void ImGuiTileSet::Shutdown()
	{
		m_TileSet.clear();
	}

	Entity ImGuiTileSet::Create2DSprite(Vec2 position, Vec2 scale, std::string_view tilepath)
	{
		Entity newEnt = g_engine.m_coordinator.CreateEntity();
		auto& trans = g_engine.m_coordinator.CreateComponent<TransformComponent>(newEnt);
		trans.setPosition(position);
		trans.setScale(scale);
		trans.setZ(100);
		auto& Sprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(newEnt);
		Sprite.setTexturePath(tilepath); 
		return newEnt;
	}

	void ImGuiTileSet::ClearTileset()
	{
		//for (auto& tile : m_GlobalTileSet)
		//{
		//	g_engine.m_coordinator.AddToDeleteQueue(tile.m_tileId);
		//}
		
		//g_engine.m_coordinator.AddToDeleteQueue(m_tileMapEnt);
		if (m_tileMapEnt != MAX_ENTITIES)
		{
			g_engine.m_coordinator.DestroyEntity(m_tileMapEnt);
		}
		m_TileSet.clear();
		m_tileMapEnt = MAX_ENTITIES;
		//m_GlobalTileSet.clear();
	}


	std::string Tile::Serialize()
	{
		return std::string();
		//std::ostringstream oss;
		//oss << m_tilePos.x << "," << m_tilePos.y << ";";
		//oss << m_texCoordMinX << "," << m_texCoordMaxX << "," << m_texCoordMinY << "," << m_texCoordMaxY << ";";
		//
		//return oss.str();
	}

	void Tile::Deserialize(std::string_view deserializeStr)
	{
		std::istringstream iss(deserializeStr.data());
		std::string str;

		//Texturename
		if (std::getline(iss, str, ';'))
		{
			m_texturename = str;
			m_tileTexture = TextureManager::instance().loadTexture(str.c_str());
		}

		//TilePosition
		if (std::getline(iss, str, ','))
		{
			m_tilePos.x = std::stof(str);
		}
		if (std::getline(iss, str, ';'))
		{
			m_tilePos.y = std::stof(str);
		}

		//TexCoordinate Values
		if (std::getline(iss, str, ','))
		{
			m_texCoordMinX = std::stof(str);
		}
		if (std::getline(iss, str, ','))
		{
			m_texCoordMaxX = std::stof(str);
		}
		if (std::getline(iss, str, ','))
		{
			m_texCoordMinY = std::stof(str);
		}
		if (std::getline(iss, str, ';'))
		{
			m_texCoordMaxY = std::stof(str);
		}

		//Collison
		if (std::getline(iss, str, ';'))
		{
			//m_collision = 
			std::stoi(str);
		}

		//BorderColor
		if (std::getline(iss, str, ','))
		{
			//m_bordercolor.w = 
			std::stof(str);
		}
		if (std::getline(iss, str, ','))
		{
			//m_bordercolor.x = 
			std::stof(str);
		}
		if (std::getline(iss, str, ','))
		{
			//m_bordercolor.y = 
			std::stof(str);
		}
		if (std::getline(iss, str, ';'))
		{
			//m_bordercolor.z = 
			std::stof(str);
		}

		//Creating a sprite
		//if (m_texturename != "Resources\\Assets\\tile.png" && m_texturename != "")
		//{
		//	//std::cout << "Create: " << m_texturename << std::endl;
		//	//Entity ent = Create2DSprite(m_tilePos, Vec2(61, 61), m_texturename);
		//	//m_tileId = ent;
		//	if (auto spriteOpt = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(ent))
		//	{
		//		SpriteComponent& sprite = spriteOpt->get();
		//		sprite.setTexCoordMinX(m_texCoordMinX);
		//		sprite.setTexCoordMinY(m_texCoordMinY);
		//		sprite.setTexCoordMaxX(m_texCoordMaxX);
		//		sprite.setTexCoordMaxY(m_texCoordMaxY);
		//	}
		//
		//	g_engine.m_coordinator.AddComponent(ent, TileMapComponent());
		//}

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

	//std::vector<Tile>& ImGuiTileSet::GetTileSet()
	//{
	//	return m_GlobalTileSet;
	//}

	void ImGuiTileSet::SaveTileMap(TileMap& globalMap)
	{
		std::vector<TileMap::iterator> deleteQueue;

		for (Tile& tile : m_TileSet)
		{
			m_globalcheck = false;

			for (TileMap::iterator iTrueTile = globalMap.begin(); iTrueTile != globalMap.end(); ++iTrueTile)
			{
				//find global tile
				if (tile.m_tilePos.x == iTrueTile->m_tilePos.x && tile.m_tilePos.y == iTrueTile->m_tilePos.y)
				{
					m_globalcheck = true; // globalcheck means FOUND

					// Tile becomes invalidated (removed).
					if (tile.m_texturename == "Resources\\Assets\\tile.png")
					{
						deleteQueue.emplace_back(iTrueTile);
					}
					else
					{
						// Add the coordinates to the global tile
						iTrueTile->m_min.x = tile.m_texCoordMinX;
						iTrueTile->m_max.x = tile.m_texCoordMaxX;
						iTrueTile->m_min.y = tile.m_texCoordMinY;
						iTrueTile->m_max.y = tile.m_texCoordMaxY;
					}
					break;
				}
			}
			
			if (m_globalcheck || tile.m_texturename == "Resources\\Assets\\tile.png")
				continue;

				// If current tile does not exist in global tile set
			TrueTile newTile;
			newTile.m_min.x = tile.m_texCoordMinX;
			newTile.m_max.x = tile.m_texCoordMaxX;
			newTile.m_min.y = tile.m_texCoordMinY;
			newTile.m_max.y = tile.m_texCoordMaxY;

			newTile.m_tilePos = tile.m_tilePos;

			globalMap.emplace_back(newTile);
			
		}

		for (TileMap::iterator it : deleteQueue)
		{
			globalMap.erase(it);
		}
	}
}