#pragma once
#include "BaseSystem.h"
#include "EventListener.h"
#include <vector>
#include <map>

namespace Rogue
{
	class CollisionTagSystem
		: public System, public EventListener
	{
		std::string m_name;
	public:
		CollisionTagSystem();

		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event& ev) override;

		void AddTag(std::string name);
		void RemoveTag(std::string name);

		void AssignTag(Entity entityToAssign, std::string tagName);
		void DeassignTag(Entity entityToAssign);
		std::string GetTag();
		std::vector<std::string>& GetTagList();
		std::map<Entity, std::string> GetEntityTagMap();

		//List of tags as integers
		std::vector<std::string> m_tagsList;
		std::map<Entity, std::string> m_entityTagsMap;
	};
}