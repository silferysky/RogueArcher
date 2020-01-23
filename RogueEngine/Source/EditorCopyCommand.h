#pragma once
#include "Precompiled.h"
#include "REEngine.h"
#include "ICommandable.h"
#include "Main.h"
namespace Rogue
{
	class EditorCopyCommand : public ICommandable
	{
	public:
		EditorCopyCommand(Entity entity);
		~EditorCopyCommand();
		virtual bool Undo() override;
		virtual bool Execute() override;
		Entity GetCopiedEntity() const;
	private:
		Entity m_copyEntity = -1;
	};
}