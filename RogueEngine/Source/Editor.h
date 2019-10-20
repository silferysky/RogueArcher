#pragma once
#include "Editor.h"
#include "EditorManager.h"
#include "REEditor.h"
#include "REEngine.h"

namespace Rogue
{
	class Editor : public System, public EventListener
	{
	public:
		Editor() :
			System(SystemID::id_EDITOR),
			m_EditorManager{ std::make_shared<EditorManager>() }
		{}

		void init() override;
		void update() override;
		void receive(Event* ev) override;
		void Shutdown();
	private:
		 std::shared_ptr<EditorManager> m_EditorManager;
	};
}
