#pragma once
#include "ILogic.h"
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
		void AddLogicInterface(Entity, ILogic*);
		void RemoveLogicInterface(Entity);

		//Basic System 
		void init();
		void update() override;
		void receive(Event* ev) override;

	private:
		std::map<Entity, ILogic*> m_entityLogicMap;
	};

}