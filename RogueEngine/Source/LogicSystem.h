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

		//Basic System 
		void init();
		void update() override;
		void receive(Event * ev) override;

		//For entity logic
		void AddLogicInterface(Entity entity, BaseAI* logicInterface);
		void RemoveLogicInterface(Entity);

		void SeekNearestWaypoint(Entity ent);

		void CreateMoveEvent(Entity ent, Vec2 vec);

	private:
		std::map<Entity, BaseAI*> m_entityLogicMap;
	};

}