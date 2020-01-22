#pragma once
#include "Precompiled.h"
#include "REEngine.h"
#include "ICommandable.h"
#include "Main.h"
namespace Rogue
{
	class EditorCreateObjectCommand : public ICommandable
	{
	public:
		EditorCreateObjectCommand();
		~EditorCreateObjectCommand();
		virtual bool Undo() override;
		virtual bool Execute() override;
	private:
		std::shared_ptr<Entity> m_objectSnapShot;
		std::weak_ptr<Entity> m_object;
		Entity m_newEntity;
	};
}