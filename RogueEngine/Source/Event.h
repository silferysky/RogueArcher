/*Start Header*************************************************************************/
/*!
\file           Event.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for Event

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

#include <string>
#include <sstream>
#include <bitset>
#include "SystemList.h"

namespace Rogue
{
#define EvCatFlag(x) (1 << x) //Defines event category as x shifted by 1 bit (since 0 is None)

	//Macros to help set Category/Type
#define SET_EVENT_TYPE(type) static EventType GetStaticType() { return EventType::##type; } virtual EventType GetEventType() const override { return type; } virtual const char* GetEventName() const override { return #type; }
#define SET_EVENT_CATEGORY(cat) virtual int GetEventCat() const override {return cat;}
#define stringify(name) #name

	enum EventType
	{
		EvTypeNone = 0,

		EvWindowClose,
		EvWindowResize,
		EvWindowFocus,
		EvWindowLostFocus,
		EvWindowMoved,

		EvAppTick,
		EvAppUpdate,
		EvAppRender,

		EvKeyPressed,
		EvKeyReleased,
		EvKeyTriggered,

		EvMouseButtonPressed,
		EvMouseButtonReleased,
		EvMouseButtonTriggered,
		EvMouseMoved,
		EvMouseScrolled,
		EvMouseDoubleClick,

		EvEntityChangeState,

		EvEntityMove,
		EvEntityTeleport,
		EvEntityAttacking,
		EvEntityDamaged,
		EvEntityDestroy,
		EvEntityHitchhike,
		EvEntityHover,

		EvFreezeControls,
		EvUnfreezeControls,

		EvResetGame,

		EvOnCollisionEnter,
		EvOnCollisionStay,
		EvOnCollisionExit,

		EvCameraShake,
		EvChangeAnimation,
		EvEntityPicked,
		EvTransition,
		EvFade,
		EvEntityChangeSprite,
		EvEntityChangeRGBA,
		EvEntityResetToCamera,
		EvZoom, 

		EvParentSet,
		EvParentReset,
		EvParentTransformUpdate,
		EvParentRotateUpdate,
		EvParentScaleUpdate,
		EvChildTransformUpdate,

		EvEditorCreateObject,
		EvEditorDeleteObject,
		EvEditorCutObject,
		EvEditorCopyObject,
		EvEditorPasteObject,
		EvEditorB
	};

	enum EventCategory
	{
		EventCatNone			= 0,
		EventCatApp				= EvCatFlag(0),			//Any event related to application (resize, movement etc)
		EventCatInput			= EvCatFlag(1),			//Any input (keyboard/mouse)
		EventCatKeyboard		= EvCatFlag(2),			//Only keyboard input
		EventCatMouse			= EvCatFlag(3),			//Only mouse input (Movement + click)
		EventCatMouseButton		= EvCatFlag(4),			//Only mouse input (Click)
		EventCatCombinedInput	= EvCatFlag(5),			//For input events that are combined (More than 1)

		EventCatEntAddCmp		= EvCatFlag(6),			//Adding component to entity

		EventCatEntity			= EvCatFlag(7),			//For entity events
		EventCatEntChangeState	= EvCatFlag(8),			//For entity behavior changing state
		EventCatEntMove			= EvCatFlag(9),			//For entity movement/teleportation
		EventCatEntAttack		= EvCatFlag(10),		//For entity attacking/taking damage
		EventCatEntDestroy		= EvCatFlag(11),		//For completely removing entities

		EventCatCollision		= EvCatFlag(12),
		EventCatTrigger			= EvCatFlag(13),

		EventCatGraphics		= EvCatFlag(14),

		EventCatParent			= EvCatFlag(15),
		EventCatChild			= EvCatFlag(16),
    
		EventCatEditor			= EvCatFlag(17),
		EventCatEditorUndo		= EvCatFlag(18)
	};

	/*enum EventMessageType
	{
		MSG_BROADCAST = 0,
		MSG_DIRECT
	};*/

	class Event
	{
		friend class EventDispatcher;

	public:

		//Functions to override
		virtual int GetEventCat() const { return (int)eventCat; };
		virtual EventType GetEventType() const { return eventType; };
		virtual const char* GetEventName() const { return "None"; };
		virtual bool Handled() const { return isHandled; };
		virtual std::string ToString() const { return GetEventName(); };
		virtual ~Event() = default;

		//Checker functions
		inline bool InCategory(EventCategory cat)
		{
			//In this case need use bit comparison since can be multiple categories
			return (cat & eventCat);
		};

		//For denoting system receivers
		void SetSystemReceivers(std::bitset<(int)SystemID::id_LASTSYS> bitset);
		void SetSystemReceivers(int bit, bool state = true);
		void SetSystemReceivers();
		std::bitset<(int)SystemID::id_LASTSYS> GetSystemReceivers() const;

		/*template<typename... ints>
		void SetSystemReceivers(int first, ints... args)
		{
			SetSystemReceivers(first);
			if constexpr(sizeof...(args)) > 0)
				SetSystemReceivers(args);
		}*/


	protected:
		EventCategory	eventCat{};				//Event Category as based on the EventCategory enum
		EventType		eventType{};				//Specific event type
		bool			isHandled = false;		//If event is handled or not
		std::bitset<(int)SystemID::id_LASTSYS>	m_systemsToReceive;		//Denotes which system receives this eevnt
	};

	//For Debugging/Logging
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}