#include "Precompiled.h"
#include "ParentChildSystem.h"
#include "Main.h"
#include "ParentEvent.h"
#include "EditorHierarchyInfo.h"

namespace Rogue
{
	ParentChildSystem::ParentChildSystem()
		: System(SystemID::id_PARENTCHILDSYSTEM) {}

	void ParentChildSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_PARENTCHILDSYSTEM, ParentChildSystem::Receive);
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<ChildComponent>());

		g_engine.m_coordinator.SetSystemSignature<ParentChildSystem>(signature);
	}

	void ParentChildSystem::Update()
	{
		for (Entity entity : m_entities)
		{
			
		}
	}

	void ParentChildSystem::Shutdown()
	{
	}

	void ParentChildSystem::Receive(Event* ev)
	{
		switch (ev->GetEventType())
		{
		case EvParentSet:
		{
			ParentSetEvent* parentEvent = dynamic_cast<ParentSetEvent*>(ev);
			ReassignParentChildFlags(parentEvent->GetChildEntity(), parentEvent->GetParentEntity());
			break;
		}
		case EvParentTransformUpdate:
		{
			ParentTransformEvent* parentEvent = dynamic_cast<ParentTransformEvent*>(ev);
			HierarchyInfo parentHierarchyInfo = g_engine.m_coordinator.GetHierarchyInfo(parentEvent->GetParentEntity());
			std::vector<Entity> entityToUpdate{};

			Vec2 transformChange{};
			int zChange = 0;

			//Change in value for all objects
			if (g_engine.m_coordinator.ComponentExists<TransformComponent>(parentEvent->GetParentEntity()))
			{
				TransformComponent parentTrans = g_engine.m_coordinator.GetComponent<TransformComponent>(parentEvent->GetParentEntity());
				transformChange = Vec2(parentEvent->GetXTransform() - parentTrans.GetPosition().x, parentEvent->GetYTransform() - parentTrans.GetPosition().y);
				zChange = parentEvent->GetZTransform() - parentTrans.GetZ();
			}

			//Picking all objects that needs updating
			entityToUpdate.push_back(parentEvent->GetParentEntity());
			AddChildToVector(entityToUpdate, parentEvent->GetParentEntity());

			for (auto& entity : entityToUpdate)
			{
				if (g_engine.m_coordinator.ComponentExists<TransformComponent>(entity))
				{
					TransformComponent& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
					trans.setPosition(trans.GetPosition() + transformChange);
					trans.setZ(trans.GetZ() + zChange);
				}
			}
			entityToUpdate.clear();

			//Updating parent of this object
			if (g_engine.m_coordinator.ComponentExists<ChildComponent>(parentEvent->GetParentEntity()))
			{
				ChildComponent& parent = g_engine.m_coordinator.GetComponent<ChildComponent>(parentEvent->GetParentEntity());
				parent.SetLocalDirty();
			}

			break;
		}
		case EvParentScaleUpdate:
		{
			ParentScaleEvent* parentEvent = dynamic_cast<ParentScaleEvent*>(ev);
			HierarchyInfo parentHierarchyInfo = g_engine.m_coordinator.GetHierarchyInfo(parentEvent->GetParentEntity());
			std::vector<Entity> entityToUpdate{};
			Vec2 scaleChange{};

			if (g_engine.m_coordinator.ComponentExists<TransformComponent>(parentEvent->GetParentEntity()))
			{
				TransformComponent parentTrans = g_engine.m_coordinator.GetComponent<TransformComponent>(parentEvent->GetParentEntity());
				scaleChange = Vec2(parentEvent->GetXScale() / parentTrans.GetScale().x, parentEvent->GetYScale() / parentTrans.GetScale().y);
			}

			if (scaleChange.x == 0.0f && scaleChange.y == 0.0f)
				break;

			entityToUpdate.push_back(parentEvent->GetParentEntity());
			AddChildToVector(entityToUpdate, parentEvent->GetParentEntity());

			for (auto& entity : entityToUpdate)
			{
				if (g_engine.m_coordinator.ComponentExists<TransformComponent>(entity))
				{
					TransformComponent& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
					trans.setScale(Vec2(trans.GetScale().x * scaleChange.x, trans.GetScale().y * scaleChange.y));
				}
			}
			entityToUpdate.clear();
			break;
		}
		case EvParentRotateUpdate:
		{
			ParentRotateEvent* parentEvent = dynamic_cast<ParentRotateEvent*>(ev);
			HierarchyInfo parentHierarchyInfo = g_engine.m_coordinator.GetHierarchyInfo(parentEvent->GetParentEntity());
			std::vector<Entity> entityToUpdate{};

			float rotateChange = 0.0f;

			if (g_engine.m_coordinator.ComponentExists<TransformComponent>(parentEvent->GetParentEntity()))
			{
				TransformComponent parentTrans = g_engine.m_coordinator.GetComponent<TransformComponent>(parentEvent->GetParentEntity());
				rotateChange = parentEvent->GetRotateScale() - parentTrans.GetRotation();
			}

			entityToUpdate.push_back(parentEvent->GetParentEntity());
			AddChildToVector(entityToUpdate, parentEvent->GetParentEntity());

			for (auto& entity : entityToUpdate)
			{
				if (g_engine.m_coordinator.ComponentExists<TransformComponent>(entity))
				{
					TransformComponent& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
					trans.setRotation(trans.GetRotation() + rotateChange);
				}
			}
			entityToUpdate.clear();

			break;
		}
		}
	}

	void ParentChildSystem::AddChildToVector(std::vector<Entity>& entityVector, Entity ParentEntity)
	{
		HierarchyInfo parentHierarchyInfo = g_engine.m_coordinator.GetHierarchyInfo(ParentEntity);
		std::vector<Entity> newVector;
		//Marking all entities that needs to be updated
		for (auto& entity : parentHierarchyInfo.m_children)
		{
			entityVector.push_back(entity);
			newVector.push_back(entity);
		}

		for (auto& entity : newVector)
		{
			AddChildToVector(entityVector, entity);
		}

		newVector.clear();
	}

	bool ParentChildSystem::CheckValidReassign(Entity child, Entity newParent)
	{
		bool isValid = true;
		HierarchyInfo it = g_engine.m_coordinator.GetHierarchyInfo(child);
		while (it.m_parent != -1 && it.m_parent != MAX_ENTITIES)
		{
			if (it.m_parent == child)
			{
				isValid = false;
				break;
			}
			it = g_engine.m_coordinator.GetHierarchyInfo(it.m_parent);
		}

		return isValid;
	}

	void ParentChildSystem::ReassignParentChildFlags(Entity child, Entity newParent)
	{
		//If invalid reassign (loop)
		if (!CheckValidReassign(child, newParent))
			return;

		//This cannot be done directly here, since it is middle of a loop.
		g_engine.m_coordinator.SetReassignParentFlags(child, newParent);
	}
}