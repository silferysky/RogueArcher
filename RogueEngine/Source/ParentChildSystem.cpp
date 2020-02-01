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
			auto& childComponent = g_engine.m_coordinator.GetComponent<ChildComponent>(entity);

			if (childComponent.GetParent() == -1 || !g_engine.m_coordinator.ComponentExists<TransformComponent>(childComponent.GetParent()))
				return;

			auto& transComponent = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
			auto& parentTransformComponent = g_engine.m_coordinator.GetComponent<TransformComponent>(childComponent.GetParent());

			//Global values is "corrupted", need to fix
			if (childComponent.IsGlobalDirty())
			{
				transComponent.setPosition(parentTransformComponent.GetPosition() - childComponent.GetPosition());
				transComponent.setZ(parentTransformComponent.GetZ() - childComponent.GetPositionZ());
				transComponent.setScale(Vec2(parentTransformComponent.GetScale().x / childComponent.GetScale().x, parentTransformComponent.GetScale().y / childComponent.GetScale().y));
				transComponent.setRotation(parentTransformComponent.GetRotation() - childComponent.GetRotation());
			}
			//Local values is "corrupted", need to fix
			else if (childComponent.IsLocalDirty())
			{
				childComponent.SetPosition(parentTransformComponent.GetPosition() + transComponent.GetPosition());
				childComponent.SetPositionZ(parentTransformComponent.GetZ() + transComponent.GetZ());
				childComponent.SetScale(Vec2(parentTransformComponent.GetScale().x * transComponent.GetScale().x, parentTransformComponent.GetScale().y * transComponent.GetScale().y));
				childComponent.SetRotation(parentTransformComponent.GetRotation() + transComponent.GetRotation());
			}
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
			ReassignParentChildFlags(parentEvent->GetParentEntity(), parentEvent->GetChildEntity());

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

	bool ParentChildSystem::CheckValidReassign(Entity newParent, Entity child)
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

	void ParentChildSystem::ReassignParentChildFlags(Entity newParent, Entity child)
	{
		//If invalid reassign (loop)
		if (!CheckValidReassign(child, newParent))
			return;

		if (!g_engine.m_coordinator.ComponentExists<ChildComponent>(child))
		{
			g_engine.m_coordinator.CreateComponent<ChildComponent>(child);
		}

		ChildComponent& childComp = g_engine.m_coordinator.GetComponent<ChildComponent>(child);
		childComp.SetLocalDirty();
		childComp.SetParent(newParent);

		//Reassigning in Hierarchy Objects

		HierarchyInfo& childInfo = g_engine.m_coordinator.GetHierarchyInfo(child);
		HierarchyInfo& newParentInfo = g_engine.m_coordinator.GetHierarchyInfo(newParent);

		if (childInfo.m_parent != -1 && childInfo.m_parent != MAX_ENTITIES)
		{
			HierarchyInfo& oldParentInfo = g_engine.m_coordinator.GetHierarchyInfo(childInfo.m_parent);

			auto end = std::remove(oldParentInfo.m_children.begin(), oldParentInfo.m_children.end(), child);
			oldParentInfo.m_children.erase(end, oldParentInfo.m_children.end());
			//oldParentInfo.m_children.erase(std::remove_if(oldParentInfo.m_children.begin(), oldParentInfo.m_children.end(), m_assignChild), oldParentInfo.m_children.end());
		}

		childInfo.m_parent = newParent;
		newParentInfo.m_children.push_back(child);
	}

	void ParentChildSystem::ResetParentChildFlags(Entity child)
	{
		if (g_engine.m_coordinator.ComponentExists<ChildComponent>(child))
		{
			g_engine.m_coordinator.RemoveComponent<ChildComponent>(child);
		}
		
		//Now doing the same for hierarchyinfo
		HierarchyInfo& childHierarchy = g_engine.m_coordinator.GetHierarchyInfo(child);

		if (childHierarchy.m_parent != -1 && childHierarchy.m_parent != MAX_ENTITIES)
		{
			HierarchyInfo& parentHierarchy = g_engine.m_coordinator.GetHierarchyInfo(childHierarchy.m_parent);
			auto end = std::remove(parentHierarchy.m_children.begin(), parentHierarchy.m_children.end(), child);
			parentHierarchy.m_children.erase(end, parentHierarchy.m_children.end());
		}
		childHierarchy.m_parent = (Entity)-1;
	}
}