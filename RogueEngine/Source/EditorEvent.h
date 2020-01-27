#pragma once
#include "Event.h"
#include "Vector2D.h"
#include "Types.h"

namespace Rogue
{
	class EditorEvent : public Event
	{
	public:
		SET_EVENT_CATEGORY(EventCatEditor)

		inline int GetEntityID() { return ID; }
		inline bool GetIsUndo() { return Undo; }

		inline void SetIsUndo(bool undo) { Undo = undo; }

	protected:
		EditorEvent(Entity id, bool undo = false)
			: ID(id), Undo{ undo } {}

		Entity ID;
		bool Undo;
	};

	class EditorCreateObjectEvent : public EditorEvent
	{
	public:
		SET_EVENT_TYPE(EvEditorCreateObject)

		EditorCreateObjectEvent(Entity id = 0)
			: EditorEvent(id) { }

	protected:
	};

}