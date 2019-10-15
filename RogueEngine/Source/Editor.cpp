#include "Editor.h"

namespace Rogue
{
	void Editor::Init()
	{
		m_EditorManager->AddEditorWindow("Viewport", new ImGuiEditorEdit{});
	}

	void Editor::Update(float dt)
	{
	}

	void Editor::Shutdown()
	{
	}
}

