/* Start Header ************************************************************************/
/*!
\file           SoulComponent.h
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions definition for SoulComponent

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	struct Soul
	{
		Soul(Entity entity, bool collected = false) :
			m_entity{ entity }, m_collected{ collected }
		{}

		Entity m_entity;
		bool m_collected;
	};
	
	class SoulComponent
		: public BaseComponent
	{
	public:
		SoulComponent() = default;

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
		void DisplayOnInspector();
	};
}