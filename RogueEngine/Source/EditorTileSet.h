#pragma once
#include "Precompiled.h"
#include "REEditor.h"

namespace Rogue
{
	struct Tile
	{
		std::string m_texturename;
		Entity m_tileId = 0;
		Vec2 m_tilePos;
		bool m_collision = false;
		Texture m_tileTexture = { 0 };
		ImVec4 m_bordercolor = { 1.0f,1.0f,1.0f,0.5f };
	};

	enum Mode
	{
		Single,
		Drag
	};

	class ImGuiTileSet : public IEditable
	{
	private:
		std::vector<Tile> m_GlobalTileSet;
		std::vector<Tile> m_TileSet;
		bool m_openWindow;
		bool m_isCollision;
		bool m_check;
		bool m_firstclicked;
		bool m_globalcheck;
		bool m_deleteTile;
		int m_tileSize;
		int m_tilesHeight;
		int m_tilesWidth;
		float m_currentTileX;
		float m_currentTileY;
		float m_minX;
		float m_minY;
		float m_maxX;
		float m_maxY;
		Texture m_currentTexture = { 0 };
		std::string m_currentPath = "None";
		

		int m_currentmode = 0;

	public:
		static ImGuiTileSet& instance()
		{
			static ImGuiTileSet instance;
			return instance;
		}
		ImGuiTileSet();
		~ImGuiTileSet() = default;
		Entity Create2DSprite(Vec2 position, Vec2 scale, std::string_view tilepath,bool iscollision);
		virtual void Init() override final;
		virtual void Update() override final;
		virtual void Shutdown() override final;
	};
}

