#pragma once
#include "Precompiled.h"
#include "REEngine.h"

namespace Rogue
{
	class EditorDeleteCommand
	{
	public:
		EditorDeleteCommand();
		~EditorDeleteCommand();
	private:
		std::shared_ptr<Entity> m_EntityPointer;
	};
}