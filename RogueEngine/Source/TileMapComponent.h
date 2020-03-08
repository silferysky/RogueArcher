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
			oss << m_min.x << "," << m_max.x << ";" << m_min.y << "," << m_max.y << ";";
		
			return oss.str();
		}

		void Deserialize(std::string deserializedString)
		{
			std::istringstream iss(deserializedString.data());
			std::string str;

			if (std::getline(iss, str, ','))
				m_tilePos.x = std::stof(str);

			if (std::getline(iss, str, ';'))
				m_tilePos.y = std::stof(str);

			if (std::getline(iss, str, ','))
				m_min.x = std::stof(str);

			if (std::getline(iss, str, ';'))
				m_max.x = std::stof(str);

			if (std::getline(iss, str, ','))
				m_min.y = std::stof(str);

			if (std::getline(iss, str, ';'))
				m_max.y = std::stof(str);
		}
	};

	class TileMapComponent : public BaseComponent
	{
		Vec2 m_UVScale;
		std::vector<TrueTile> m_tilemap;
	public:
		TileMapComponent() :
			m_UVScale{0.2f, 0.2f}
		{}

		// Does nothing. Cao ni ma
		std::string Serialize()								{ return std::string(); }
		void Deserialize(std::string_view deserializeStr)	{}

		std::vector<TrueTile>& GetTileMap()					{ return m_tilemap; }
		Vec2 GetUVScale() const								{ return m_UVScale; }

		void SetUVScale(Vec2 scale)							{ m_UVScale = scale; }
	};
}