/*Start Header*************************************************************************/
/*!
\file           GraphicsEvent.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for GraphicsEvent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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

	class TransitionEvent : public Event
	{
	public:
		SET_EVENT_CATEGORY(EventCatGraphics)
		SET_EVENT_TYPE(EvTransition)

		TransitionEvent(bool transitingIn)
			: m_transitingIn{transitingIn} {}

		inline bool GetTransitionType() const { return m_transitingIn; }
	private:
		bool m_transitingIn;
	};

	class FadeEvent : public Event
	{
	public:
		SET_EVENT_CATEGORY(EventCatGraphics);
		SET_EVENT_TYPE(EvFade);

		FadeEvent(Entity ent, float factor, bool transitAfterFade = true) : 
			m_entity{ ent }, m_factor{ factor }, m_transitAfterFade{ transitAfterFade } {}

		inline Entity GetEntityToFade() const { return m_entity; }
		inline float GetFadeFactor() const { return m_factor; }
		inline bool GetTransitAfterFade() const { return m_transitAfterFade; }

	private:
		Entity m_entity;
		float m_factor;
		bool m_transitAfterFade;
	};

	class ZoomEvent : public Event
	{
	public:
		SET_EVENT_CATEGORY(EventCatGraphics);
		SET_EVENT_TYPE(EvZoom);

		ZoomEvent(float factor, float duration)
			: m_factor{ factor }, m_duration{ duration }{}

		inline float GetFactor() const { return m_factor; }
		inline float GetDuration() const { return m_duration; }

	private:
		float m_factor, m_duration;
	};
}