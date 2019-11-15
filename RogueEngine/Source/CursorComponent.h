#pragma once
#include "BaseComponent.h"
#include "Vector2D.h"

namespace Rogue
{
	class CursorComponent
		: public BaseComponent
	{
	public:
		CursorComponent() = default;
		~CursorComponent() = default;

		Vec2 m_positionOffset;

		const Vec2& GetPosOffset();
		void SetPosOffset(const Vec2& offset);

		std::string_view Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
	};
}