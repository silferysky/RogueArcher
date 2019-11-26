#pragma once
#include "IEditable.h"
#include "REEngine.h"
#include "Main.h"
#include "EditorHierarchyInfo.h"
#include "EventDispatcher.h"
#include "KeyEvent.h"

namespace Rogue
{
	class ImGuiEditorHierarchy : public IEditable
	{
	private:
		std::vector<Entity>& m_currentVector;
	public:
		ImGuiEditorHierarchy();
		~ImGuiEditorHierarchy();

		virtual void Init() override;
		virtual void Update() override;
		virtual void Shutdown() override;
		
	};
}