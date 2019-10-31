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

		void Init() override;
		void Update() override;
		void Receive(Event* ev) override final;
		void Shutdown() override;
	private:
		 std::shared_ptr<EditorManager> m_EditorManager;
		 std::vector<HierarchyInfo>& m_currentVector = g_engine.m_coordinator.GetActiveObjects();
	};
}
