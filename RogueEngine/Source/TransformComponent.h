#pragma once
#include "BaseComponent.h"
#include "Vector2D.h"
#include "AABB.h"

namespace Rogue
{
	class TransformComponent
		: public BaseComponent
	{
		Vec2 m_position;
		Vec2 m_scale;
		float m_rotation;

		int m_fakeZ;

		AABB m_pickArea;
	public:
		TransformComponent(const Vec2& pos = { 0.0f, 0.0f }, const Vec2& scale = { 1.0f, 1.0f },
			float rot = 0.0f, int Z = 0, const AABB& aabb = AABB{});

		void setPosition(const Vec2& pos);
		void offSetPosition(const Vec2& pos);
		void setScale(const Vec2& scale);
		void offSetScale(const Vec2& scale);
		void setRotation(float rot);
		void offSetRotation(float rot);
		void setPickArea(const AABB& aabb);
		void setZ(int z);

		Vec2 GetPosition() const;
		Vec2 GetScale() const;
		float GetRotation() const;
		const AABB& GetPickArea() const;
		int GetZ() const;

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
		void DisplayOnInspector();
	};
}
