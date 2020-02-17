#pragma once
#include "Precompiled.h"
#include "REEditor.h"

namespace Rogue
{
	struct TileSet
	{
		std::string_view m_texturename;
		Entity m_tileId;
		Vec2 m_tilePos;
		bool m_collision = false;
		Texture m_tileTexture;
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
		std::vector<TileSet> m_GlobalTileSet;
		std::vector<TileSet> m_TileSet;
		bool m_openWindow = true;
		int m_tileSize = 100;
		bool m_isCollision = false;
		int m_tilesHeight = 0;
		int m_tilesWidth = 0;
		float m_currentTileX = 0;
		float m_currentTileY = 0;
		bool m_check = true;
		bool m_globalcheck = false;
		Texture m_currentTexture;
		std::string m_currentPath = "None";
		bool m_firstclicked = false;
		float m_minX;
		float m_minY;
		float m_maxX;
		float m_maxY;
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

