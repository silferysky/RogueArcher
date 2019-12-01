/*Start Header************************************************************************ /
/*!
\file           CursorComponent.h
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for CursorComponent

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
	class CursorComponent
		: public BaseComponent
	{
	public:
		CursorComponent() = default;
		~CursorComponent() = default;

		Vec2 m_positionOffset;

		const Vec2& GetPosOffset();
		void SetPosOffset(const Vec2& offset);

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
	};
}