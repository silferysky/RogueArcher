#pragma once
#include "Event.h"
#include "Types.h"

namespace Rogue
{
	class ParentEvent : public Event
	{
	public:
		inline Entity GetParentEntity() { return parentEntity; }
	protected:
		ParentEvent(Entity parentEnt)
			: parentEntity{ parentEnt } {}
	private:
		Entity parentEntity;
	};

	class ParentTransformEvent : public ParentEvent
	{
	public:
		SET_EVENT_CATEGORY(EventCatParent)
		SET_EVENT_TYPE(EvParentTransformUpdate)

		ParentTransformEvent(Entity parentEnt, float xTransform = 0.0f, float yTransform = 0.0f, float zTransform = 0.0f)
			: ParentEvent(parentEnt),x{ xTransform }, y{ yTransform }, z{ zTransform } {}

		inline float GetXTransform() { return x; }
		inline float GetYTransform() { return y; }
		inline float GetZTransform() { return z; }
	private:
		float x, y, z;
	};

	class ParentScaleEvent : public ParentEvent
	{
	public:
		SET_EVENT_CATEGORY(EventCatParent)
		SET_EVENT_TYPE(EvParentTransformUpdate)

		ParentScaleEvent(Entity parentEnt, float xTransform = 0.0f, float yTransform = 0.0f)
			: ParentEvent(parentEnt), x{ xTransform }, y{ yTransform } {}

		inline float GetXScale() { return x; }
		inline float GetYScale() { return y; }
	private:
		float x, y;
	};

	class ParentRotateEvent : public ParentEvent
	{
	public:
		SET_EVENT_CATEGORY(EventCatParent)
		SET_EVENT_TYPE(EvParentTransformUpdate)

		ParentRotateEvent(Entity parentEnt, float rotate = 0.0f)
			: ParentEvent(parentEnt), rotateValue{ rotate } {}

		inline float GetRotateScale() { return rotateValue; }
	private:
		float rotateValue;
	};
}