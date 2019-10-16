#include "Editor.h"

namespace Rogue
{
	void Editor::Init()
	{

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

	void Editor::Update()
	{
		m_EditorManager->Update();
	}

	void Editor::Shutdown()
	{
		m_EditorManager->Shutdown();
	}
}

