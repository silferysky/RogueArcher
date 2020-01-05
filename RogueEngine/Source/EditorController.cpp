#include "Precompiled.h"
#include "EditorController.h"

namespace Rogue
{
	EditorController::EditorController()
	{
	}
	EditorController::~EditorController()
	{
		ClearUndoRedoStack();
	}
	void EditorController::ExecuteCommand(std::shared_ptr<ICommandable> command)
	{
		m_redoStack.clear();
		command->Execute();
		m_undoStack.push_back(command);
	}

	void EditorController::Undo()
	{
		if (!m_undoStack.size())
			return;

		m_undoStack.back()->Undo();
		m_redoStack.push_back(m_undoStack.back());
		m_undoStack.pop_back();
	}

	void EditorController::Redo()
	{
		if (!m_redoStack.size())
			return;
		m_redoStack.front()->Redo();
		m_undoStack.push_back(m_redoStack.front());
		m_redoStack.pop_back();
	}

	void EditorController::ClearUndoRedoStack()
	{
		if (m_undoStack.size() != NULL) 
		{   
			/*for (ICommandable* i : m_undoStack) 
			{
				delete i;
			}*/
			m_undoStack.clear();
		}

		if (m_redoStack.size() != NULL) 
		{ 
			/*for (ICommandable* i : m_redoStack) 
			{
				delete i;
			}*/
			m_redoStack.clear();
		}
	}
	void EditorController::InsertUndo(ICommandable* command)
	{
	}
	void EditorController::InsertRedo(ICommandable* command)
	{
	}
}