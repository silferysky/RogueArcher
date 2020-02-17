#pragma once
#include "Precompiled.h"
#include "REEditor.h"

namespace Rogue
{
	struct Tile
		: public ISerializable
	{
		std::string m_texturename;
		Entity m_tileId = 0;
		Vec2 m_tilePos;
		bool m_collision = false;
		Texture m_tileTexture = { 0 };
		ImVec4 m_bordercolor = { 1.0f,1.0f,1.0f,0.5f };

		virtual std::string Serialize() override
		{
			std::ostringstream oss;
			oss << m_texturename << ";";
			oss << m_tilePos.x << "," << m_tilePos.y << ";";
			oss << m_collision << ";";
			oss << m_bordercolor.w << "," << m_bordercolor.x << "," << m_bordercolor.y << "," << m_bordercolor.z << ";";

			return oss.str();
		}

		virtual void Deserialize(std::string_view deserializeStr) override
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

		}
	};

	enum Mode
	{
		Single,
		Drag
	};

	class ImGuiTileSet : public IEditable, public ISerializable
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

		virtual std::string Serialize() override;
		virtual void Deserialize(std::string_view deserializeStr) override;
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

