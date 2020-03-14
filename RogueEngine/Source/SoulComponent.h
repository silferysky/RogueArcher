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