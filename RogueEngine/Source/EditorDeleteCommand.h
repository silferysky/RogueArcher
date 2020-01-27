#pragma once
#include "Precompiled.h"
#include "REEngine.h"
#include "ICommandable.h"
#include "Main.h"

namespace Rogue
{
	class EditorDeleteCommand :public ICommandable
	{
	public:
		EditorDeleteCommand(Entity entity);
		~EditorDeleteCommand();
		virtual bool Undo() override;
		virtual bool Execute() override;
	private:
		Entity m_entity = -1;
		std::vector<Entity> m_deletedEntities;
	};
}