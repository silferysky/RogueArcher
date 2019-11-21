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
			System(SystemID::id_EDITOR)
		{}

		void Init() override;
		void Update() override;
		void Receive(Event* ev) override final;
		void Shutdown() override;
	private:
		 std::vector<HierarchyInfo>& m_currentVector = g_engine.m_coordinator.GetActiveObjects();
	};
}
