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
		void Receive(Event& ev) override;

		//Accessing all entities that are children. Assume they all have ChildComponents
		void AddChildToVector(std::vector<Entity>& entityVector, Entity ParentEntity);

		//Same as the update loop, but callable
		void ApplyParentChildTransform(Entity entity);

		//Parent/Child Assignment
		bool CheckValidReassign(Entity newParent, Entity child);
		void ReassignParentChildFlags(Entity newParent, Entity child);
		void ResetParentChildFlags(Entity child);
	};
}