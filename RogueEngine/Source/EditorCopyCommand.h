#pragma once
#include "Precompiled.h"
#include "REEngine.h"
#include "ICommandable.h"
namespace Rogue
{
	class EditorCopyCommand : public ICommandable
	{
		EditorCopyCommand();
		~EditorCopyCommand();
		virtual bool Undo() override;
		virtual bool Execute() override;
	};
}