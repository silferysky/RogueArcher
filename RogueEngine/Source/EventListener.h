/*Start Header*************************************************************************/
/*!
\file           EventListener.cpp
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
#include "logger.h"
#include "Event.h"

namespace Rogue
{
	//Used to override template events
	class BaseEventListener
	{
	public:
		virtual ~BaseEventListener() {};
	};

	//EventListener denotes the derived class is actively listening for events
	class EventListener : public BaseEventListener
	{
	public:
		virtual ~EventListener() override {};
		virtual void Receive(Event& event) = 0;
	};

	using LISTENER_HANDLER = std::function<void(Event&)>;
}
#define REGISTER_LISTENER(id, func)	LISTENER_HANDLER hand = std::bind(&func, this, std::placeholders::_1); \
									EventDispatcher::instance().AddListener(id, hand)