/* Start Header ************************************************************************/
/*!
\file           CameraComponent.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for CameraComponent

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

namespace Rogue
{
	class CameraComponent
		: public BaseComponent
	{
		Vec2 m_position = Vec2(0.0f, 0.0f);
		bool m_isActive = true;
		Vec2 m_offset = Vec2(0.0f, 0.0f);
		// TODO: orthographic size
		// view matrix
		// render target 
	public:
		CameraComponent() = default;
		CameraComponent(const Vec2& pos);

		void setPosition(const Vec2& pos);
		void offSetPosition(const Vec2& pos);

		void setIsActive(const bool& isActive);
		bool getIsActive() const;

		void setOffset(const Vec2& offset);
		Vec2 getOffset() const;

		Vec2 getPosition() const;
		void DisplayOnInspector();
		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
	};
}
