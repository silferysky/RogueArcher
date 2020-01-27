#include "Precompiled.h"
#include "EditorController.h"

namespace Rogue
{
	EditorController::EditorController():m_undoStack{},m_redoStack{}
	{
	}
	EditorController::~EditorController()
	{
		ClearUndoRedoStack();
	}
	void EditorController::ExecuteCommand(ICommandable* command)
	{
		
		if (command->Execute())
		{
			m_undoStack.pop_back();
		}

		m_undoStack.push_back(command);
		if (!m_redoStack.size())
		{
			m_redoStack.clear();
		}
	}

	void EditorController::UndoCommand()
	{
		if (!m_undoStack.size())
			return;

		if(m_undoStack.back()->Undo())
			m_redoStack.push_back(m_undoStack.back());
		m_undoStack.pop_back();
	}

	void EditorController::RedoCommand()
	{
		if (!m_redoStack.size())
			return;
		if(m_redoStack.front()->Execute())
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