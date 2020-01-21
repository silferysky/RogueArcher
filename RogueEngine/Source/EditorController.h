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
		void ExecuteCommand(std::shared_ptr<ICommandable> command);
		void UndoCommand();
		void RedoCommand();
		void ClearUndoRedoStack();

		void InsertUndo(ICommandable* command);
		void InsertRedo(ICommandable* command);

	private:
		std::vector<std::shared_ptr<ICommandable>> m_undoStack;
		std::vector<std::shared_ptr<ICommandable>> m_redoStack;
		size_t m_undoLimit;

	};
}
