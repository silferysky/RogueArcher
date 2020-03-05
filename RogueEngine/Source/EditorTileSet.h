#pragma once
#include "Precompiled.h"
#include "REEditor.h"

namespace Rogue
{
	struct Tile // 30 bytes!!
		: public ISerializable
	{
		std::string m_texturename;
		Entity m_tileId = 0;
		Vec2 m_tilePos;
		bool m_collision = false;
		float m_texCoordMinX = 0.0f;
		float m_texCoordMaxX = 1.0f;
		float m_texCoordMinY = 0.0f;
		float m_texCoordMaxY = 1.0f;
		Texture m_tileTexture = { 0 };
		ImVec4 m_bordercolor = { 1.0f,1.0f,1.0f,0.5f };

		virtual std::string Serialize() override;
		virtual void Deserialize(std::string_view deserializeStr) override;
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
		bool m_hasTextureChanged;
		bool m_groundtag;
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
		std::vector<Tile>& GetTileSet();

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
		void ClearTileset();
		virtual void Init() override final;
		virtual void Update() override final;
		virtual void Shutdown() override final;
	};
}

