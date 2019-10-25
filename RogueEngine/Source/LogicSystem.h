#pragma once
#include "BaseAI.h"
#include "EventListener.h"

namespace Rogue
{
	class LogicSystem :
		public System, public EventListener
	{
	public:
		//Logic System constructors/destructors
		LogicSystem();
		~LogicSystem() = default;

		//For entity logic
		void AddLogicInterface(Entity, BaseAI*);
		void RemoveLogicInterface(Entity);

		void SeekNearestWaypoint(Entity ent);

		//Basic System 
		void init();
		void update() override;
		void receive(Event* ev) override;

	private:
		std::map<Entity, BaseAI*> m_entityLogicMap;
	};

}