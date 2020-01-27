#include "Precompiled.h"
#include "EditorCopyCommand.h"


namespace Rogue
{
	EditorCopyCommand::EditorCopyCommand(Entity entity):m_copyEntity(entity)
	{
	}
	EditorCopyCommand::~EditorCopyCommand()
	{
	}
	bool EditorCopyCommand::Undo()
	{
		if (m_copyEntity < 0)
		{
			return false;
		}
		g_engine.m_coordinator.AddToDeleteQueue(m_copyEntity);
		return true;
	}
	bool EditorCopyCommand::Execute()
	{
		if (m_copyEntity < 0)
		{
			return false;
		}
		g_engine.m_coordinator.clone(m_copyEntity);
		m_copyEntity = -1;
		return true;
	}
	Entity EditorCopyCommand::GetCopiedEntity() const
	{
		return m_copyEntity;
	}
}


