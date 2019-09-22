#pragma once

#include <map>
#include "RogueEngine.h"
#include "Event.h"

class EventDispatcher;
class EventListener
{
public:
	friend class EventDispatcher;

	void AddListener(SYSTEMID ID, BaseSystem* sys)
	{
		if (ListenerMap[ID] != nullptr)
		{
			ListenerMap.insert(std::pair<SYSTEMID, BaseSystem*>(ID, sys));
			RE_CORE_INFO("Added new key to ListenerMap");
		}
		else
		{
			RE_CORE_WARN("ATTEMPTED TO DUPLICATE KEY");
		}

	}

	void RemoveListener(SYSTEMID ID)
	{
		//No need to check for nonexistent pointers since map doesn't care
		unsigned i = ListenerMap.size();
		ListenerMap.erase(ID);

		if (i > ListenerMap.size())
			RE_CORE_INFO("Removed key from ListenerMap");
		else
			RE_CORE_INFO("Attempted but failed to remove key from ListenerMap");
	}

	std::map<SYSTEMID, BaseSystem*> GetMap()
	{
		return ListenerMap;
	}

private:
	static std::map<SYSTEMID, BaseSystem*> ListenerMap;
}eventListener;

std::map<SYSTEMID, BaseSystem*> EventListener::ListenerMap = std::map<SYSTEMID, BaseSystem*>();