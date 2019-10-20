#pragma once
#include "BaseComponent.h"
#include "Vector2D.h"

namespace Rogue
{
	class TransformComponent
		: public BaseComponent
	{
		Vec2 m_position = Vec2(0.0f, 0.0f);
		Vec2 m_scale = Vec2(0.5f, 0.5f);
		float m_rotation = 0.0f;

	public:
		void setPosition(const Vec2& pos);
		void offSetPosition(const Vec2& pos);
		void setScale(const Vec2& scale);
		void offSetScale(const Vec2& scale);
		void setRotation(const float rot);
		void offSetRotation(const float rot);

		Vec2 getPosition() const;
		Vec2 getScale() const;
		float getRotation() const;

		std::string Serialize();
		void Deserialize(std::string toDeserialize);
	};
}
