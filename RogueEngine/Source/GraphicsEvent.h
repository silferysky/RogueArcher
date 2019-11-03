#pragma once
#include "Event.h"

namespace Rogue
{
	class CameraShakeEvent : public Event
	{
	public:
		SET_EVENT_CATEGORY(EventCatGraphics)
		SET_EVENT_TYPE(EvCameraShake)

		CameraShakeEvent(float magnitudeValue = 5.0f)
			: magnitude {magnitudeValue}{ SetSystemReceivers((int)SystemID::id_CAMERASYSTEM);}
		inline float getMagnitude(){ return magnitude; }

	private:

		float magnitude;
	};

	class ChangeAnimationEvent : public Event
	{
	public:
		SET_EVENT_CATEGORY(EventCatGraphics)
		SET_EVENT_TYPE(EvChangeAnimation)

		ChangeAnimationEvent() = default;
	};
}