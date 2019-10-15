#pragma once
#include "Editor.h"
#include "EditorManager.h"
#include "REEditor.h"
namespace Rogue
{
	class Editor
	{
	public:
		Editor() :
			m_EditorManager{ std::make_shared<EditorManager>() }
		{}

		void Init();
		void Update();
		void Shutdown();
	private:
		 std::shared_ptr<EditorManager> m_EditorManager;
	};
}
