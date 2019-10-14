#pragma once
#include "Editor.h"
#include "EditorManager.h"

namespace Rogue
{
	class Editor
	{
	public:
		void Init();
		void Update(float dt);
		void Shutdown();
	private:
		EditorManager* m_EditorManager;
	};
}
