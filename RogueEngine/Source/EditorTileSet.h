#pragma once
#include "Precompiled.h"
#include "REEditor.h"

namespace Rogue
{
	struct TileSet
	{
		std::string m_texturename;
		GLuint m_tileId;
		Vec2 m_minPos;
		Vec2 m_MaxPos;
	};

	class ImGuiTileSet : public IEditable
	{
	private:
		std::vector<TileSet> m_TileSet;
		bool m_openWindow = true;
		int m_tileSize = 64;
		float m_columnWidth;
		ImVec2 m_mousePos;
		bool m_isCollision;
		float m_viewportWidth;
		float m_viewportHeight;
		int m_tilesHeight = 0;
		int m_tilesWidth = 0;

	public:
		static ImGuiTileSet& instance()
		{
			static ImGuiTileSet instance;
			return instance;
		}
		ImGuiTileSet() :m_TileSet() {}
		~ImGuiTileSet() = default;
		Entity Create2DSprite();
		virtual void Init() override final;
		virtual void Update() override final;
		virtual void Shutdown() override final;
	};
}

