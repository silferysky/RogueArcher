#include "Editor.h"

namespace Rogue
{
	void Editor::Init()
	{
		m_EditorManager->AddEditorWindow("Assets", std::make_shared<ImGuiAssets>());
		m_EditorManager->AddEditorWindow("Component", std::make_shared<ImGuiComponent>());
		m_EditorManager->AddEditorWindow("Console", std::make_shared<ImGuiConsole>());
		m_EditorManager->AddEditorWindow("Edit", std::make_shared<ImGuiEditorEdit>());
		m_EditorManager->AddEditorWindow("File", std::make_shared<ImGuiEditorFile>());
		m_EditorManager->AddEditorWindow("Game Object", std::make_shared<ImGuiGameObject>());
		m_EditorManager->AddEditorWindow("Hierarchy", std::make_shared<ImGuiEditorHierarchy>());
		m_EditorManager->AddEditorWindow("Inspector", std::make_shared<ImGuiInspector>());
		m_EditorManager->AddEditorWindow("Project", std::make_shared<ImGuiProject>());
		m_EditorManager->AddEditorWindow("Viewport", std::make_shared<ImGuiEditorViewport>());

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

