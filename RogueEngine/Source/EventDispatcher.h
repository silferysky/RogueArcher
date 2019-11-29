#pragma once
#include <map>
#include "EventListener.h"
#include "BaseSystem.h"

namespace Rogue
{
	class EventDispatcher
	{
	public:
		static EventDispatcher& instance()
		{
			static EventDispatcher instance;
			return instance;
		}

		EventDispatcher() = default;
		~EventDispatcher() = default;

		void Init();

		//Queue functions
		Event* GetQueueHead();
		Event* GetQueueHeadDelayed();
		void CombineQueue();
		void CombineQueueCmd(Event& e);

		//Listener functions
		void AddListener(SystemID ID, LISTENER_HANDLER handler);
		void RemoveAllListener();
		void RemoveListener(SystemID ID);
		std::map<SystemID, LISTENER_HANDLER> GetMap();

		//Adding / Removing Events
		void AddEvent(Event* e);
		void AddEventDelayed(Event* e);
		void ResetEvents();

		void Update();

		//Dispatch sends it to the relavent system to execute event
		void DispatchEvent(Event* toHandle);

	private:
		std::queue<Event*> EventQueue;
		std::queue<Event*> DelayedEventQueue;
		std::map<SystemID, LISTENER_HANDLER> ListenerMap;
		bool isCombiningQueue = false;
	};
}