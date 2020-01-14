#pragma once
#include "Event.h"

namespace Rogue
{
	class ParentTransformEvent : public Event
	{
	public:
		SET_EVENT_CATEGORY(EventCatParent)
		SET_EVENT_TYPE(EvParentTransformUpdate)

		ParentTransformEvent(float xTransform = 0.0f, float yTransform = 0.0f, float zTransform = 0.0f)
			:x{ xTransform }, y{ yTransform }, z{ zTransform } {}

		inline float GetXTransform() { return x; }
		inline float GetYTransform() { return y; }
		inline float GetZTransform() { return z; }
	private:
		float x, y, z;
	};

	class ParentScaleEvent : public Event
	{
	public:
		SET_EVENT_CATEGORY(EventCatParent)
		SET_EVENT_TYPE(EvParentTransformUpdate)

		ParentScaleEvent(float xTransform = 0.0f, float yTransform = 0.0f)
			:x{ xTransform }, y{ yTransform } {}

		inline float GetXScale() { return x; }
		inline float GetYScale() { return y; }
	private:
		float x, y;
	};

	class ParentRotateEvent : public Event
	{
	public:
		SET_EVENT_CATEGORY(EventCatParent)
		SET_EVENT_TYPE(EvParentTransformUpdate)

		ParentRotateEvent(float rotate = 0.0f)
			:rotateValue{ rotate } {}

		inline float GetRotateScale() { return rotateValue; }
	private:
		float rotateValue;
	};
}