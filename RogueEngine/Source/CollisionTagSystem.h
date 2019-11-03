#pragma once
#include "BaseSystem.h"
#include "EventListener.h"
#include <vector>
#include <map>

namespace Rogue
{
	class CollisionTagSystem
		: public System, public EventListener
	{
	public:
		CollisionTagSystem();

		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event* ev) override;

		std::vector<int> m_tagsList;
		std::map<Entity, int> m_EntityTagsList;
	};
}