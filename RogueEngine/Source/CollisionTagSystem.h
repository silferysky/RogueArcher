#pragma once
#include "BaseSystem.h"
#include "EventListener.h"
#include <vector>
#include <map>
#include "CollisionTags.h"

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

		void AddTag(std::string name);
		void RemoveTag(std::string name);
		void RemoveTag(int tag);

		void AssignTag(Entity entityToAssign, std::string tagName);
		void DeassignTag(Entity entityToAssign);

		std::vector<CollisionTag> GetTagList();
		std::map<Entity, CollisionTag> GetEntityTagMap();

		//List of tags as integers
		std::vector<CollisionTag> m_tagsList;
		std::map<Entity, CollisionTag> m_entityTagsMap;
	};
}