#pragma once
#include "Precompiled.h"
#include "ICommandable.h"

namespace Rogue
{
	class EditorController
	{
	public:
		EditorController();
		~EditorController();
		void ExecuteCommand(ICommandable* command);
		void UndoCommand();
		void RedoCommand();
		void ClearUndoRedoStack();

		void InsertUndo(ICommandable* command);
		void InsertRedo(ICommandable* command);

	private:
		std::vector<ICommandable*> m_undoStack;
		std::vector<ICommandable*> m_redoStack;
		size_t m_undoLimit = 256;

	};
}
