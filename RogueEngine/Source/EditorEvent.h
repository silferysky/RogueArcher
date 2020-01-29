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
		inline void SetEntityID(Entity id) { ID = id; }

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

	class EditorDeleteObjectEvent : public EditorEvent
	{
	public:
		SET_EVENT_TYPE(EvEditorDeleteObject)

		EditorDeleteObjectEvent(Entity id = 0) : EditorEvent(id){}
	};

	class EditorCopyObjectEvent : public EditorEvent
	{
	public:
		SET_EVENT_TYPE(EvEditorCopyObject)

		EditorCopyObjectEvent(Entity id = 0) : EditorEvent(id) {}
	};

	class EditorCutObjectEvent : public EditorEvent
	{
	public:

	};
	class EditorPasteObjectEvent : public EditorEvent
	{
	public:
		SET_EVENT_TYPE(EvEditorPasteObject)
		EditorPasteObjectEvent(Entity id = 0) : EditorEvent(id) {}
	};
}