#include "Editor.h"
#include "EventDispatcher.h"
#include "EventListener.h"


namespace Rogue
{
	void Editor::init()
	{
		Signature signature;
		g_Engine.m_coordinator.SetSystemSignature<Editor>(signature);
		REGISTER_LISTENER(SystemID::id_EDITOR, Editor::receive);
		m_EditorManager->AddEditorWindow<ImGuiEditorFile>("File");
		m_EditorManager->AddEditorWindow<ImGuiEditorEdit>("Edit");
		m_EditorManager->AddEditorWindow<ImGuiComponent>("Component");
		m_EditorManager->AddEditorWindow<ImGuiConsole>("Console");
		m_EditorManager->AddEditorWindow<ImGuiGameObject>("Game Object");
		m_EditorManager->AddEditorWindow<ImGuiEditorHierarchy>("Hierarchy");
		m_EditorManager->AddEditorWindow<ImGuiInspector>("Inspector");
		m_EditorManager->AddEditorWindow<ImGuiProject>("Project");
		m_EditorManager->AddEditorWindow<ImGuiEditorViewport>("Viewport");

		m_EditorManager->Init();
	}

	void Editor::update()
	{
		m_EditorManager->Update();
	}

	void Editor::receive(Event* ev)
	{

	}

	void Editor::Shutdown()
	{
		m_EditorManager->Shutdown();
	}
}

