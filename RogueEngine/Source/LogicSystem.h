#pragma once
#include "BaseAI.h"
#include "EventListener.h"
#include <memory>
#include <vector>

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
		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event* ev) override;

		//For entity logic
		void AddLogicInterface(Entity entity, std::shared_ptr<BaseAI> logicInterface);
		void RemoveLogicInterface(Entity);
		void ClearLogicInterface();

		void RemoveExcessAI();
		void AddExcessAI();

		//void SeekNearestWaypoint(Entity ent);

		void CreateMoveEvent(Entity ent, Vec2 vec);

	private:
		std::map<Entity, std::vector<std::shared_ptr<BaseAI>>> m_entityLogicMap;
	};

}