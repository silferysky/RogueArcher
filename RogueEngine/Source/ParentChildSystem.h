#pragma once
#include "BaseSystem.h"
#include "EventListener.h"
#include "ChildComponent.h"

namespace Rogue
{
	class ParentChildSystem
		: public System, public EventListener
	{
	public:
		ParentChildSystem();
		~ParentChildSystem() = default;

		//Basic System 
		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event* ev) override;

		//Parent Child access
		void AddChildToVector(std::vector<Entity>& entityVector, Entity ParentEntity);

		//Parent/Child Assignment
		bool CheckValidReassign(Entity newParent, Entity child);
		void ReassignParentChildFlags(Entity newParent, Entity child);
		void ResetParentChildFlags(Entity child);
	};
}