#pragma once
#include "BaseComponent.h"
#include "REMath.h"

namespace Rogue
{
	struct TrueTile
	{
		// Position
		Vec2 m_tilePos;

		// UV coordinates
		Vec2 m_min;
		Vec2 m_max;


		std::string Serialize()
		{
			std::ostringstream oss;
			oss << m_tilePos.x << "," << m_tilePos.y << ";";
			oss << m_min.x << "," << m_max.x << "," << m_min.y << "," << m_max.y << ";";
		
			return oss.str();
		}
	};

	class TileMapComponent : BaseComponent
	{
		std::vector<TrueTile> m_tilemap;

	public:
		// Does nothing. Cao ni ma
		std::string Serialize() { return std::string(); }
		void Deserialize(std::string_view deserializeStr) {}

		std::vector<TrueTile>& GetTileMap() { return m_tilemap; }
	};
}