#pragma once
#include "BaseComponent.h"
#include "Vector2D.h"

namespace Rogue
{
	class ChildComponent
		: public BaseComponent
	{
	public:

		ChildComponent() = default;
		~ChildComponent() = default;

		//Transform
		void SetTransform(Vec2 transform);
		void SetTransform(float x, float y);
		Vec2 GetTransform() const;

		void SetTransformZ(float z);
		float GetTransformZ() const;

		//Scale
		void SetScale(Vec2 scale);
		void SetScale(float x, float y);
		Vec2 GetScale() const;

		//Rotate
		void SetRotate(float rotate);
		float GetRotate() const;

	private:
		Vec2 m_transform;
		float m_transformZ;
		Vec2 m_scale;
		float m_rotate;
	};
}