#pragma once
#include "Precompiled.h"
#include "REEngine.h"

namespace Rogue
{
	class EditorDeleteCommand :public ICommandable
	{
	public:
		EditorDeleteCommand();
		~EditorDeleteCommand();
		virtual bool Undo() override;
		virtual bool Execute() override;
	private:
		std::shared_ptr<Entity> m_EntityPointer;
	};
}