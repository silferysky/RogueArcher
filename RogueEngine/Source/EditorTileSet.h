#pragma once
#include "Precompiled.h"
#include "REEditor.h"

namespace Rogue
{
	struct TileSet
	{
		std::string m_texturename;
		int m_tileId;
		Vec2 m_minPos;
		Vec2 m_MaxPos;
	};

	class ImGuiTileSet : public IEditable
	{
	private:
		std::vector<TileSet> m_TileSet;
		bool m_openWindow = true;
		const int m_tileSize = 64;
		float m_columnWidth;
		ImVec2 m_mousePos;
		bool m_isCollision;
		float m_viewportWidth;
		float m_viewportHeight;

	public:
		static ImGuiTileSet& instance()
		{
			static ImGuiTileSet instance;
			return instance;
		}
		ImGuiTileSet() :m_TileSet() {}
		~ImGuiTileSet() = default;

		virtual void Init() override final;
		virtual void Update() override final;
		virtual void Shutdown() override final;
	};
}

