/* Start Header ************************************************************************/
/*!
\file           TransformComponent.h
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for TransformComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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

		bool m_modified;
	public:
		TransformComponent(const Vec2& pos = { 0.0f, 0.0f }, const Vec2& scale = { 1.0f, 1.0f },
			float rot = 0.0f, int Z = 0, const AABB& aabb = AABB{}, bool modified = false);

		void setPosition(const Vec2& pos);
		void offSetPosition(const Vec2& pos);
		void setScale(const Vec2& scale);
		void offSetScale(const Vec2& scale);
		void setRotation(float rot);
		void offSetRotation(float rot);
		void setPickArea(const AABB& aabb);
		void setZ(int z);
		void setModified(bool isMod);

		Vec2 GetPosition() const;
		Vec2 GetScale() const;
		float GetRotation() const;
		const AABB& GetPickArea() const;
		int GetZ() const;
		bool GetIsModified() const;

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
		void DisplayOnInspector();
		void DisplayOnInspectorWithParent();
	};
}
