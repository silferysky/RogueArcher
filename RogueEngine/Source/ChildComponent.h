/* Start Header ************************************************************************/
/*!
\file           ChildComponent.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions definitions for ChildComponent

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "Vector2D.h"
#include "BaseComponent.h"

namespace Rogue
{
	class ChildComponent
		: public BaseComponent
	{
	public:
	
		ChildComponent();
		~ChildComponent() = default;

		//From BaseComponent
		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
		void DisplayOnInspector();
	
		//For dirty parts
		void SetLocalDirty();
		void SetGlobalDirty();
		void ResetLocalDirty();
		void ResetGlobalDirty();
		bool IsLocalDirty() const;
		bool IsGlobalDirty() const;

		//For Following
		void SetIsFollowing(bool follow);
		bool IsFollowing() const;

		void SetParent(Entity ent);
		void ResetParent();
		Entity GetParent() const;

		//Position
		void SetPosition(Vec2 position);
		void SetPosition(float x, float y);
		Vec2 GetPosition() const;
	
		void SetPositionZ(int z);
		int GetPositionZ() const;
	
		//Scale
		void SetScale(Vec2 scale);
		void SetScale(float x, float y);
		Vec2 GetScale() const;
	
		//Rotation
		void SetRotation(float rotation);
		float GetRotation() const;
	
	private:
		bool m_globalDirty;
		bool m_localDirty;
		bool m_isFollowing;

		Entity m_parent;

		Vec2 m_position;
		int m_positionZ;
		Vec2 m_scale;
		float m_rotation;
	};
}
