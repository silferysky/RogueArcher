#pragma once
#include "BaseComponent.h"
#include "Vector2D.h"

namespace Rogue
{
	class CameraComponent
		: public BaseComponent
	{
		Vec2 m_position = Vec2(0.0f, 0.0f);
	public:
		CameraComponent() = default;
		CameraComponent(const Vec2& pos);

		void setPosition(const Vec2& pos);
		void offSetPosition(const Vec2& pos);

		Vec2 getPosition() const;

		std::string Serialize();
		void Deserialize(std::string toDeserialize);
	};
}