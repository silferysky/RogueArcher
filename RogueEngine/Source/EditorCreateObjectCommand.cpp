#include "Precompiled.h"
#include "EditorCreateObjectCommand.h"

namespace Rogue
{
	EditorCreateObjectCommand::EditorCreateObjectCommand():m_objectSnapShot(nullptr)
	{
		
	}
	EditorCreateObjectCommand::~EditorCreateObjectCommand()
	{
	}
	bool EditorCreateObjectCommand::Undo()
	{
		g_engine.m_coordinator.DestroyEntity(m_newEntity);
		return true;
		//return false;
	}
	bool EditorCreateObjectCommand::Execute()
	{
		m_newEntity = SceneManager::instance().Create2DSprite();
		//if (!m_objectSnapShot)
		//{
		//	return false;
		//}
		//
		//m_objectSnapShot = std::make_shared<Entity>(m_newEntity);
		return true;
	}
}