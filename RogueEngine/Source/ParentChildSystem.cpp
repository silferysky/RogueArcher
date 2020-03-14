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

			if (childComponent.GetParent() == MAX_ENTITIES || !g_engine.m_coordinator.ComponentExists<TransformComponent>(childComponent.GetParent()))
				return;

			auto& transComponent = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
			auto& parentTransformComponent = g_engine.m_coordinator.GetComponent<TransformComponent>(childComponent.GetParent());

			//Local values is "corrupted", need to fix
			if (childComponent.IsLocalDirty())
			{
				childComponent.SetPosition(transComponent.GetPosition() - parentTransformComponent.GetPosition());
				childComponent.SetPositionZ(transComponent.GetZ() - parentTransformComponent.GetZ());
				childComponent.SetRotation(transComponent.GetRotation() - parentTransformComponent.GetRotation());

				// Special case for player, don't flip scale.
				if (entity == PLAYER_STATUS.GetPlayerEntity())
				{
					float x = 1.0f;

					if (parentTransformComponent.GetScale().x < 0.0f)
						x = -1.0f;

					g_engine.m_coordinator.GetSystem<GraphicsSystem>()->SetPlayerX(x);
				}
				else
					childComponent.SetScale(Vec2(transComponent.GetScale().x / parentTransformComponent.GetScale().x, transComponent.GetScale().y / parentTransformComponent.GetScale().y));
				
				childComponent.ResetLocalDirty();
			}

			//Global values is "corrupted", need to fix
			else if (childComponent.IsGlobalDirty())
			{
				transComponent.setPosition(childComponent.GetPosition() + parentTransformComponent.GetPosition());
				transComponent.setZ(childComponent.GetPositionZ() + parentTransformComponent.GetZ());
				transComponent.setRotation(childComponent.GetRotation() + parentTransformComponent.GetRotation());

				// Special case for player, don't flip scale.
				if (entity == PLAYER_STATUS.GetPlayerEntity())
				{
					float x = 1.0f;

					if (parentTransformComponent.GetScale().x < 0.0f)
						x = -1.0f;

					g_engine.m_coordinator.GetSystem<GraphicsSystem>()->SetPlayerX(x);
				}
				else
					transComponent.setScale(Vec2(childComponent.GetScale().x * parentTransformComponent.GetScale().x, childComponent.GetScale().y * parentTransformComponent.GetScale().y));
			

				std::vector<Entity> toUpdate;
				AddChildToVector(toUpdate, entity);
				for (auto& ent : toUpdate)
					g_engine.m_coordinator.GetComponent<ChildComponent>(ent).SetGlobalDirty();

				toUpdate.clear();

				childComponent.ResetGlobalDirty();
			}
			//Following only happens if no local or global changes, but you just want to update it
			else if (childComponent.IsFollowing())
			{
				transComponent.setPosition(childComponent.GetPosition() + parentTransformComponent.GetPosition());
				transComponent.setZ(childComponent.GetPositionZ() + parentTransformComponent.GetZ());
				transComponent.setRotation(childComponent.GetRotation() + parentTransformComponent.GetRotation());

				// Special case for player, don't flip scale.
				if (entity == PLAYER_STATUS.GetPlayerEntity())
				{
					float x = 1.0f;

					if (parentTransformComponent.GetScale().x < 0.0f)
						x = -1.0f;
					
					g_engine.m_coordinator.GetSystem<GraphicsSystem>()->SetPlayerX(x);
				}
				else
					transComponent.setScale(Vec2(childComponent.GetScale().x * parentTransformComponent.GetScale().x, childComponent.GetScale().y * parentTransformComponent.GetScale().y));
				
			}

			if (transComponent.GetScale().x == 0.0f)
				transComponent.setScale(Vec2(1.0f, transComponent.GetScale().y));
			if (transComponent.GetScale().y == 0.0f)
				transComponent.setScale(Vec2(transComponent.GetScale().x, 1.0f));

		}
	}

	void ParentChildSystem::Shutdown()
	{
	}

	void ParentChildSystem::Receive(Event& ev)
	{
		switch (ev.GetEventType())
		{
		case EvParentSet:
		{
			ParentSetEvent& parentEvent = dynamic_cast<ParentSetEvent&>(ev);
			ReassignParentChildFlags(parentEvent.GetParentEntity(), parentEvent.GetChildEntity());

			break;
		}
		case EvParentReset:
		{
			ParentResetEvent& parentEvent = dynamic_cast<ParentResetEvent&>(ev);
			ResetParentChildFlags(parentEvent.GetChildEntity());

			break;
		}
		case EvParentTransformUpdate:
		{
			ParentTransformEvent& parentEvent = dynamic_cast<ParentTransformEvent&>(ev);

			if (g_engine.m_coordinator.GetHierarchyInfo(parentEvent.GetParentEntity()).m_children.size() > 0)
			{
				std::vector<Entity> temp;

				//By right Parent don't need, since it will be set via Transform (AKA no need change since no child), or via ChildComponent
				temp.push_back(parentEvent.GetParentEntity());
				AddChildToVector(temp, parentEvent.GetParentEntity());

				for (auto it : temp)
				{
					if (g_engine.m_coordinator.ComponentExists<ChildComponent>(it))
						g_engine.m_coordinator.GetComponent<ChildComponent>(it).SetGlobalDirty();
				}
			}
			//HierarchyInfo parentHierarchyInfo = g_engine.m_coordinator.GetHierarchyInfo(parentEvent.GetParentEntity());
			//std::vector<Entity> entityToUpdate{};

			//Vec2 transformChange{};
			//int zChange = 0;

			////Change in value for all objects
			//if (g_engine.m_coordinator.ComponentExists<TransformComponent>(parentEvent.GetParentEntity()))
			//{
			//	TransformComponent parentTrans = g_engine.m_coordinator.GetComponent<TransformComponent>(parentEvent.GetParentEntity());
			//	transformChange = Vec2(parentEvent.GetXTransform() - parentTrans.GetPosition().x, parentEvent.GetYTransform() - parentTrans.GetPosition().y);
			//	zChange = parentEvent.GetZTransform() - parentTrans.GetZ();
			//}

			////Picking all objects that needs updating
			//entityToUpdate.push_back(parentEvent.GetParentEntity());
			//AddChildToVector(entityToUpdate, parentEvent.GetParentEntity());

			//for (auto& entity : entityToUpdate)
			//{
			//	if (g_engine.m_coordinator.ComponentExists<TransformComponent>(entity))
			//	{
			//		TransformComponent& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
			//		trans.setPosition(trans.GetPosition() + transformChange);
			//		trans.setZ(trans.GetZ() + zChange);
			//	}
			//}
			//entityToUpdate.clear();

			////Updating parent of this object
			//if (g_engine.m_coordinator.ComponentExists<ChildComponent>(parentEvent.GetParentEntity()))
			//{
			//	ChildComponent& parent = g_engine.m_coordinator.GetComponent<ChildComponent>(parentEvent.GetParentEntity());
			//	parent.SetLocalDirty();
			//}

			break;
		}
		case EvParentScaleUpdate:
		{
			ParentScaleEvent& parentEvent = dynamic_cast<ParentScaleEvent&>(ev);

			if (g_engine.m_coordinator.GetHierarchyInfo(parentEvent.GetParentEntity()).m_children.size() > 0)
			{
				std::vector<Entity> temp;

				//By right Parent don't need, since it will be set via Transform (AKA no need change since no child), or via ChildComponent
				temp.push_back(parentEvent.GetParentEntity());
				AddChildToVector(temp, parentEvent.GetParentEntity());

				for (auto it : temp)
				{
					if (g_engine.m_coordinator.ComponentExists<ChildComponent>(it))
						g_engine.m_coordinator.GetComponent<ChildComponent>(it).SetGlobalDirty();
				}
			}
			//HierarchyInfo parentHierarchyInfo = g_engine.m_coordinator.GetHierarchyInfo(parentEvent.GetParentEntity());
			//std::vector<Entity> entityToUpdate{};
			//Vec2 scaleChange{};

			//if (g_engine.m_coordinator.ComponentExists<TransformComponent>(parentEvent.GetParentEntity()))
			//{
			//	TransformComponent parentTrans = g_engine.m_coordinator.GetComponent<TransformComponent>(parentEvent.GetParentEntity());
			//	scaleChange = Vec2(parentEvent.GetXScale() / parentTrans.GetScale().x, parentEvent.GetYScale() / parentTrans.GetScale().y);
			//}

			//if (scaleChange.x == 0.0f && scaleChange.y == 0.0f)
			//	break;

			//entityToUpdate.push_back(parentEvent.GetParentEntity());
			//AddChildToVector(entityToUpdate, parentEvent.GetParentEntity());

			//for (auto& entity : entityToUpdate)
			//{
			//	if (g_engine.m_coordinator.ComponentExists<TransformComponent>(entity))
			//	{
			//		TransformComponent& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
			//		trans.setScale(Vec2(trans.GetScale().x * scaleChange.x, trans.GetScale().y * scaleChange.y));
			//	}
			//}
			//entityToUpdate.clear();
			break;
		}
		case EvParentRotateUpdate:
		{
			ParentRotateEvent& parentEvent = dynamic_cast<ParentRotateEvent&>(ev);

			if (g_engine.m_coordinator.GetHierarchyInfo(parentEvent.GetParentEntity()).m_children.size() > 0)
			{
				std::vector<Entity> temp;

				//By right Parent don't need, since it will be set via Transform (AKA no need change since no child), or via ChildComponent
				temp.push_back(parentEvent.GetParentEntity());
				AddChildToVector(temp, parentEvent.GetParentEntity());

				for (auto it : temp)
				{
					if (g_engine.m_coordinator.ComponentExists<ChildComponent>(it))
						g_engine.m_coordinator.GetComponent<ChildComponent>(it).SetGlobalDirty();
				}
			}
			//HierarchyInfo parentHierarchyInfo = g_engine.m_coordinator.GetHierarchyInfo(parentEvent.GetParentEntity());
			//std::vector<Entity> entityToUpdate{};

			//float rotateChange = 0.0f;

			//if (g_engine.m_coordinator.ComponentExists<TransformComponent>(parentEvent.GetParentEntity()))
			//{
			//	TransformComponent parentTrans = g_engine.m_coordinator.GetComponent<TransformComponent>(parentEvent.GetParentEntity());
			//	rotateChange = parentEvent.GetRotateScale() - parentTrans.GetRotation();
			//}

			//entityToUpdate.push_back(parentEvent.GetParentEntity());
			//AddChildToVector(entityToUpdate, parentEvent.GetParentEntity());

			//for (auto& entity : entityToUpdate)
			//{
			//	if (g_engine.m_coordinator.ComponentExists<TransformComponent>(entity))
			//	{
			//		TransformComponent& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
			//		trans.setRotation(trans.GetRotation() + rotateChange);
			//	}
			//}
			//entityToUpdate.clear();

			break;
		}
		case EvChildTransformUpdate:
		{
			ChildTransformEvent& childEvent = dynamic_cast<ChildTransformEvent&>(ev);

			if (g_engine.m_coordinator.ComponentExists<ChildComponent>(childEvent.GetChildEntity()))
			{
				if (childEvent.GetDirtyGlobal())
					g_engine.m_coordinator.GetComponent<ChildComponent>(childEvent.GetChildEntity()).SetGlobalDirty();
				else
					g_engine.m_coordinator.GetComponent<ChildComponent>(childEvent.GetChildEntity()).SetLocalDirty();

			}

			//To modify all children
			if (g_engine.m_coordinator.GetHierarchyInfo(childEvent.GetChildEntity()).m_children.size() > 0)
			{
				std::vector<Entity> temp;

				//By right Parent don't need, since it will be set via Transform (AKA no need change since no child), or via ChildComponent
				temp.push_back(childEvent.GetChildEntity());
				AddChildToVector(temp, childEvent.GetChildEntity());

				for (auto it : temp)
				{
					if (g_engine.m_coordinator.ComponentExists<ChildComponent>(it))
						g_engine.m_coordinator.GetComponent<ChildComponent>(it).SetGlobalDirty();
				}
			}
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
			if (g_engine.m_coordinator.ComponentExists<ChildComponent>(entity))
			{
				entityVector.push_back(entity);
				newVector.push_back(entity);
			}
		}

		for (auto& entity : newVector)
		{
			AddChildToVector(entityVector, entity);
		}

		newVector.clear();
	}

	void ParentChildSystem::ApplyParentChildTransform(Entity entity)
	{
		if (!g_engine.m_coordinator.ComponentExists<ChildComponent>(entity))
			return;

		auto& childComponent = g_engine.m_coordinator.GetComponent<ChildComponent>(entity);

		if (childComponent.GetParent() == MAX_ENTITIES || !g_engine.m_coordinator.ComponentExists<TransformComponent>(childComponent.GetParent()))
			return;

		auto& transComponent = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
		auto& parentTransformComponent = g_engine.m_coordinator.GetComponent<TransformComponent>(childComponent.GetParent());

		//Local values is "corrupted", need to fix
		if (childComponent.IsLocalDirty())
		{
			childComponent.SetPosition(transComponent.GetPosition() - parentTransformComponent.GetPosition());
			childComponent.SetPositionZ(transComponent.GetZ() - parentTransformComponent.GetZ());
			childComponent.SetScale(Vec2(transComponent.GetScale().x / parentTransformComponent.GetScale().x, transComponent.GetScale().y / parentTransformComponent.GetScale().y));
			childComponent.SetRotation(transComponent.GetRotation() - parentTransformComponent.GetRotation());
			childComponent.ResetLocalDirty();
		}

		//Global values is "corrupted", need to fix
		else if (childComponent.IsGlobalDirty())
		{
			transComponent.setPosition(childComponent.GetPosition() + parentTransformComponent.GetPosition());
			transComponent.setZ(childComponent.GetPositionZ() + parentTransformComponent.GetZ());
			transComponent.setScale(Vec2(childComponent.GetScale().x * parentTransformComponent.GetScale().x, childComponent.GetScale().y * parentTransformComponent.GetScale().y));
			transComponent.setRotation(childComponent.GetRotation() + parentTransformComponent.GetRotation());

			std::vector<Entity> toUpdate;
			AddChildToVector(toUpdate, entity);
			for (auto& ent : toUpdate)
				g_engine.m_coordinator.GetComponent<ChildComponent>(ent).SetGlobalDirty();

			toUpdate.clear();

			childComponent.ResetGlobalDirty();
		}
		//Following only happens if no local or global changes, but you just want to update it
		else if (childComponent.IsFollowing())
		{
			transComponent.setPosition(childComponent.GetPosition() + parentTransformComponent.GetPosition());
			transComponent.setZ(childComponent.GetPositionZ() + parentTransformComponent.GetZ());
			transComponent.setScale(Vec2(childComponent.GetScale().x * parentTransformComponent.GetScale().x, childComponent.GetScale().y * parentTransformComponent.GetScale().y));
			transComponent.setRotation(childComponent.GetRotation() + parentTransformComponent.GetRotation());
		}

		if (transComponent.GetScale().x == 0.0f)
			transComponent.setScale(Vec2(1.0f, transComponent.GetScale().y));
		if (transComponent.GetScale().y == 0.0f)
			transComponent.setScale(Vec2(transComponent.GetScale().x, 1.0f));

	}

	bool ParentChildSystem::CheckValidReassign(Entity newParent, Entity child)
	{
		//Safety catch
		if (child == MAX_ENTITIES)
			return false;

		bool isValid = true;
		HierarchyInfo it = g_engine.m_coordinator.GetHierarchyInfo(newParent);
		int count = 0;

		//If newParent is child, or assigning to self, it is invalid
		if (it.m_Entity == child || child == newParent)
			isValid = false;

		//Doing a check if any of parent's parents is the child value
		while (isValid && it.m_parent != MAX_ENTITIES)
		{
			if (count > 10)
			{
				isValid = false;
			}
			if (it.m_parent == child)
			{
				isValid = false;
			}
			it = g_engine.m_coordinator.GetHierarchyInfo(it.m_parent);
			++count;
		}

		return isValid;
	}

	void ParentChildSystem::ReassignParentChildFlags(Entity newParent, Entity child)
	{
		if (!CheckValidReassign(newParent, child))
			return;

		//Ensures that ChildComponent exists
		if (!g_engine.m_coordinator.ComponentExists<ChildComponent>(child))
		{
			g_engine.m_coordinator.CreateComponent<ChildComponent>(child);
		}

		//Local data has to change
		ChildComponent& childComp = g_engine.m_coordinator.GetComponent<ChildComponent>(child);
		childComp.SetLocalDirty();
		childComp.SetParent(newParent);
		ApplyParentChildTransform(child);

		//Reassigning in Hierarchy Objects
		HierarchyInfo& childInfo = g_engine.m_coordinator.GetHierarchyInfo(child);
		HierarchyInfo& newParentInfo = g_engine.m_coordinator.GetHierarchyInfo(newParent);

		//Safety check to make sure that child isn't a parent of parent before reassignment
		bool isValid = true;
		HierarchyInfo it = g_engine.m_coordinator.GetHierarchyInfo(newParent);
		while (isValid && it.m_parent != MAX_ENTITIES)
		{
			if (it.m_parent == child)
			{
				isValid = false;
			}
			it = g_engine.m_coordinator.GetHierarchyInfo(it.m_parent);
		}


		if (childInfo.m_parent != MAX_ENTITIES)
		{
			HierarchyInfo& oldParentInfo = g_engine.m_coordinator.GetHierarchyInfo(childInfo.m_parent);
			auto end = std::remove(oldParentInfo.m_children.begin(), oldParentInfo.m_children.end(), child);
			oldParentInfo.m_children.erase(end, oldParentInfo.m_children.end());
			//oldParentInfo.m_children.erase(std::remove_if(oldParentInfo.m_children.begin(), oldParentInfo.m_children.end(), m_assignChild), oldParentInfo.m_children.end());
		}

		childInfo.m_parent = newParent;
		newParentInfo.m_children.push_back(child);
		if (childInfo.m_tag == "Player")
		{
			//childComp.SetIsFollowing(false);
		}
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
		childHierarchy.m_parent = MAX_ENTITIES;
	}
}