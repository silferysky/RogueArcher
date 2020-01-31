#pragma once
#include "Event.h"
#include "Types.h"

namespace Rogue
{
	class ParentEvent : public Event
	{
	public:
		SET_EVENT_CATEGORY(EventCatParent)
		inline Entity GetParentEntity() { return parentEntity; }
		inline bool GetDirtyGlobal() { return dirtyGlobal; }

	protected:
		ParentEvent(Entity parentEnt, bool isGlobal)
			: parentEntity{ parentEnt }, dirtyGlobal{ isGlobal } {}
	private:
		Entity parentEntity;
		bool dirtyGlobal;
	};

	class ParentSetEvent : public ParentEvent
	{
	public:
		SET_EVENT_TYPE(EvParentSet)

		ParentSetEvent(Entity parentEnt, Entity childEnt)
			: ParentEvent(parentEnt, false), childEntity{ childEnt } {}

		inline Entity GetChildEntity() { return childEntity; }

	private:
		Entity childEntity;
	};

	class ParentTransformEvent : public ParentEvent
	{
	public:
		SET_EVENT_TYPE(EvParentTransformUpdate)

		ParentTransformEvent(Entity parentEnt, bool dirtyGlobal, float xTransform = 0.0f, float yTransform = 0.0f, float zTransform = 0.0f)
			: ParentEvent(parentEnt, dirtyGlobal),x{ xTransform }, y{ yTransform }, z{ zTransform } {}

		inline float GetXTransform() { return x; }
		inline float GetYTransform() { return y; }
		inline float GetZTransform() { return z; }
	private:
		float x, y, z;
	};

	class ParentScaleEvent : public ParentEvent
	{
	public:
		SET_EVENT_TYPE(EvParentTransformUpdate)

		ParentScaleEvent(Entity parentEnt, bool dirtyGlobal, float xTransform = 0.0f, float yTransform = 0.0f)
			: ParentEvent(parentEnt, dirtyGlobal), x{ xTransform }, y{ yTransform } {}

		inline float GetXScale() { return x; }
		inline float GetYScale() { return y; }
	private:
		float x, y;
	};

	class ParentRotateEvent : public ParentEvent
	{
	public:
		SET_EVENT_TYPE(EvParentTransformUpdate)

		ParentRotateEvent(Entity parentEnt, bool dirtyGlobal, float rotate = 0.0f)
			: ParentEvent(parentEnt, dirtyGlobal), rotateValue{ rotate } {}

		inline float GetRotateScale() { return rotateValue; }
	private:
		float rotateValue;
	};
}