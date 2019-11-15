#include "Precompiled.h"
#include "Editor.h"
#include "EventDispatcher.h"
#include "EventListener.h"


namespace Rogue
{
	void Editor::Init()
	{
		Signature signature;
		g_engine.m_coordinator.SetSystemSignature<Editor>(signature);
		REGISTER_LISTENER(SystemID::id_EDITOR, Editor::Receive);
		m_EditorManager->AddEditorWindow<ImGuiEditorFile>("File");
		m_EditorManager->AddEditorWindow<ImGuiEditorEdit>("Edit");
		m_EditorManager->AddEditorWindow<ImGuiComponent>("Component");
		m_EditorManager->AddEditorWindow<ImGuiGameObject>("Game Object");
		m_EditorManager->AddEditorWindow<ImGuiEditorHierarchy>("Hierarchy");
		m_EditorManager->AddEditorWindow<ImGuiInspector>("Inspector");
		//m_EditorManager->AddEditorWindow<ImGuiProject>("Project");
		m_EditorManager->AddEditorWindow<ImGuiEditorViewport>("Viewport");
		m_EditorManager->AddEditorWindow<ImGuiEditorSettings>("Settings");
		m_EditorManager->AddEditorWindow<ImGuiConsole>("Console");
		m_EditorManager->AddEditorWindow<ImGuiProfiler>("Profiler");
		m_EditorManager->Init();
	}

	void Editor::Update()
	{
		m_EditorManager->Update();
	}

	void Editor::Receive(Event* ev)
	{
		KeyTriggeredEvent* keytriggeredevent = dynamic_cast<KeyTriggeredEvent*>(ev);
		KeyPress keycode = keytriggeredevent->GetKeyCode();
		if (keytriggeredevent->GetKeyCode() == KeyPress::Numpad8)
		{
			Entity selected = 0;
			for (auto& i : m_currentVector)
			{
				if (i.m_selected == true)
					selected = i.m_Entity;
			}

			if (!g_engine.m_coordinator.ComponentExists<TransformComponent>(selected))
				return;

			for (int i = 0; i < 1500; ++i)
			{
				Vec2 Position = g_engine.m_coordinator.GetComponent<TransformComponent>(selected).getPosition();
				Position.x = (float)(rand() % 1500);
				Position.y = (float)(rand() % 1500);
				g_engine.m_coordinator.GetComponent<TransformComponent>(selected).setPosition(Position);
				g_engine.m_coordinator.clone(selected);
			}
		}
	}

	void Editor::Shutdown()
	{
		m_EditorManager->Shutdown();
	}
}

