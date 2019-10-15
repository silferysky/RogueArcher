#include "Editor.h"

namespace Rogue
{
	void Editor::Init()
	{
		ImGuiAssets * Assets = new ImGuiAssets;
		m_EditorManager->AddEditorWindow("Assets", Assets);
		m_EditorManager->AddEditorWindow("Component", new ImGuiComponent{});
		m_EditorManager->AddEditorWindow("Console", new ImGuiConsole{});
		m_EditorManager->AddEditorWindow("Edit", new ImGuiEditorEdit{});
		m_EditorManager->AddEditorWindow("File", new ImGuiEditorFile{});
		m_EditorManager->AddEditorWindow("Game Object", new ImGuiGameObject{});
		m_EditorManager->AddEditorWindow("Hierarchy", new ImGuiEditorHierarchy{});
		m_EditorManager->AddEditorWindow("Inspector", new ImGuiInspector{});
		m_EditorManager->AddEditorWindow("Project", new ImGuiProject{});
		m_EditorManager->AddEditorWindow("Viewport", new ImGuiEditorViewport{});

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

