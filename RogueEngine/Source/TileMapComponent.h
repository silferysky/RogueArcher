#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	struct TrueTile
	{
		// Position
		Vec2 m_tilePos;

		// UV coordinates
		Vec2 m_min;
		Vec2 m_max;
	};

	class TileMapComponent : BaseComponent
	{
		std::vector<TrueTile> m_tilemap;

	public:
		// Does nothing. Cao ni ma
		std::string Serialize() { return std::string(); }
		void Deserialize(std::string_view deserializeStr) {}

		std::vector<TrueTile>& GetTileMap() { return m_tilemap; }
		TrueTile&
	};
}