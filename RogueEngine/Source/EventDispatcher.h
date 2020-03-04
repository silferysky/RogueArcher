/*Start Header*************************************************************************/
/*!
\file           EventDispatcher.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for EventDispatcher

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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
		~EventDispatcher();

		void Init();

		//Queue functions
		Event& GetQueueHead();
		//Event& GetQueueHeadDelayed();
		//void CombineQueue();
		//void CombineQueueCmd(Event& e);

		//Listener functions
		void AddListener(SystemID ID, LISTENER_HANDLER handler);
		void RemoveAllListener();
		void RemoveListener(SystemID ID);
		std::map<SystemID, LISTENER_HANDLER> GetMap();

		//Adding / Removing Events
		void AddEvent(Event& e);
		//void AddEventDelayed(Event& e);
		void ResetEvents();

		void Update();

		//Dispatch sends it to the relavent system to execute event
		void DispatchEvent(Event& toHandle);

	private:
		std::queue<std::reference_wrapper<Event>> EventQueue;
		//std::queue<Event&> DelayedEventQueue;
		std::map<SystemID, LISTENER_HANDLER> ListenerMap;
		//bool isCombiningQueue = false;
	};
}