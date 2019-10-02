#pragma once

#include <string>
#include <sstream>

#define EvCatFlag(x) (1 << x) //Defines event category as x shifted by 1 bit (since 0 is None)

//Macros to help set Category/Type
#define SET_EVENT_TYPE(type) static EventType GetStaticType() { return EventType::##type; } virtual EventType GetEventType() const override {return type;} virtual const char* GetEventName() const override { return #type; }
#define SET_EVENT_CATEGORY(cat) virtual int GetEventCat() const override {return cat;}

enum EventType
{
	EventTypeNone = 0,

	WindowClose,
	WindowResize,
	WindowFocus,
	WindowLostFocus,
	WindowMoved,

	AppTick,
	AppUpdate,
	AppRender,

	KeyPressed,
	KeyReleased,

	MouseButtonPressed,
	MouseButtonReleaseed,
	MouseMoved,
	MouseScrolled
};

enum EventCategory
{
	EventCatNone = 0,
	EventCatApp			= EvCatFlag(0),			//Any event related to application (resize, movement etc)
	EventCatInput		= EvCatFlag(1),			//Any input (keyboard/mouse)
	EventCatKeyboard	= EvCatFlag(2),			//Only keyboard input
	EventCatMouse 		= EvCatFlag(3),			//Only mouse input (Movement + click)
	EventCatMouseButton	= EvCatFlag(4)			//Only mouse input (Click)
};

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

	//Checker functions
	inline bool InCategory(EventCategory cat) 
	{
		//In this case need use bit comparison since can be multiple categories
		return (cat & eventCat);
	};

protected:
	EventCategory eventCat;			//Event Category as based on the EventCategory enum
	EventType eventType;			//Specific event type
	std::string eventName;			//Name of event
	bool isHandled = false;			//If event is handled or not
};

//For Debugging/Logging
inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
	return os << e.ToString();
}